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
    packageName: "io.qt.uic.ui4"
    defaultSuperClass: "QtObject"
    targetName: "QtJambiUIC"
    module: "qtjambi.uic"
    description: "QtJambi User Interface Compiler"
    noExports: true

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_UIC();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.End
        Text{content: "initialize_meta_info_UIC();"}
    }
    
    RequiredLibrary{
        name: "QtDesigner"
    }
    
    ObjectType{
        name: "DomUI"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementButtonGroups()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementConnections()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementCustomWidgets()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementDesignerdata()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementIncludes()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementLayoutDefault()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementLayoutFunction()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementResources()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSlots()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementTabStops()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementWidget()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementButtonGroups()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementConnections()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementCustomWidgets()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementDesignerdata()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementIncludes()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementLayoutDefault()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementLayoutFunction()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementResources()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSlots()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementTabStops()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementWidget()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementButtonGroups(DomButtonGroups*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementConnections(DomConnections*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementCustomWidgets(DomCustomWidgets*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementDesignerdata(DomDesignerData*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementIncludes(DomIncludes*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementLayoutDefault(DomLayoutDefault*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementLayoutFunction(DomLayoutFunction*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementResources(DomResources*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSlots(DomSlots*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementTabStops(DomTabStops*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementWidget(DomWidget*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomIncludes"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomInclude"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomResources"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomResource"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomActionGroup"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        implementing: "DomAttributeInterface"
    }
    
    ObjectType{
        name: "DomAction"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        implementing: "DomAttributeInterface"
    }
    
    ObjectType{
        name: "DomActionRef"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomButtonGroup"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        implementing: "DomAttributeInterface"
    }
    
    ObjectType{
        name: "DomButtonGroups"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomCustomWidgets"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomHeader"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomCustomWidget"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementHeader()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementPropertyspecifications()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSizeHint()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSlots()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementHeader()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementPropertyspecifications()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSizeHint()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSlots()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementHeader(DomHeader*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementPropertyspecifications(DomPropertySpecifications*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSizeHint(DomSize*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSlots(DomSlots*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomLayoutDefault"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomLayoutFunction"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomTabStops"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomLayout"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        implementing: "DomAttributeInterface"
    }
    
    ObjectType{
        name: "DomLayoutItem"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        EnumType{
            name: "Kind"
            InjectCode{
                target: CodeClass.Java
                position: Position.Comment
                Text{content: "@hidden"}
            }
        }
        ModifyFunction{
            signature: "takeElementLayout()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSpacer()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementWidget()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementLayout()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSpacer()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementWidget()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementLayout(DomLayout*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSpacer(DomSpacer*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementWidget(DomWidget*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomRow"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomColumn"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomItem"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomWidget"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        implementing: "DomAttributeInterface"
    }
    
    ObjectType{
        name: "DomSpacer"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        implementing: "DomAttributeInterface"
    }
    
    ObjectType{
        name: "DomColor"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomGradientStop"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementColor()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementColor()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementColor(DomColor*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomGradient"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomBrush"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        EnumType{
            name: "Kind"
            InjectCode{
                target: CodeClass.Java
                position: Position.Comment
                Text{content: "@hidden"}
            }
        }
        ModifyFunction{
            signature: "setElementColor(DomColor*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementGradient(DomGradient*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementTexture(DomProperty*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeElementColor()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementGradient()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementTexture()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementColor()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementGradient()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementTexture()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementColor()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementGradient()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementTexture()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "DomColorRole"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "setElementBrush(DomBrush*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeElementBrush()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementBrush()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "DomColorGroup"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomFont"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomPalette"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementActive()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementDisabled()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementInactive()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementActive()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementDisabled()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementInactive()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementActive(DomColorGroup*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementDisabled(DomColorGroup*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementInactive(DomColorGroup*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomPoint"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomRect"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomLocale"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomSizePolicy"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomSize"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomDate"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomTime"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomDateTime"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomStringList"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomResourcePixmap"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomResourceIcon"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementActiveOff()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementActiveOn()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementDisabledOff()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementDisabledOn()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementNormalOff()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementNormalOn()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSelectedOff()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSelectedOn()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementActiveOff()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementActiveOn()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementDisabledOff()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementDisabledOn()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementNormalOff()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementNormalOn()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSelectedOff()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSelectedOn()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementSelectedOn(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSelectedOff(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementNormalOn(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementNormalOff(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementDisabledOn(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementDisabledOff(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementActiveOn(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementActiveOff(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomString"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomPointF"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomRectF"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomSizeF"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomChar"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomUrl"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementString()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementString()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementString(DomString*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomProperty"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }

        EnumType{
            name: "Kind"
            InjectCode{
                target: CodeClass.Java
                position: Position.Comment
                Text{content: "@hidden"}
            }
        }
        ModifyFunction{
            signature: "takeElementBrush()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementChar()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementColor()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementDate()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementDateTime()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementFont()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementIconSet()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementLocale()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementPalette()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementPixmap()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementPoint()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementPointF()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementRect()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementRectF()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSize()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSizeF()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementSizePolicy()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementString()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementStringList()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementTime()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "takeElementUrl()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementBrush()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementChar()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementColor()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementDate()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementDateTime()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementFont()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementIconSet()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementLocale()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementPalette()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementPixmap()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementPoint()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementPointF()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementRect()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementRectF()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSize()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSizeF()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementSizePolicy()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementString()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementStringList()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementTime()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "elementUrl()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementBrush(DomBrush*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementChar(DomChar*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementColor(DomColor*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementDate(DomDate*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementDateTime(DomDateTime*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementFont(DomFont*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementIconSet(DomResourceIcon*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementLocale(DomLocale*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementPalette(DomPalette*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementPixmap(DomResourcePixmap*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementPoint(DomPoint*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementPointF(DomPointF*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementRect(DomRect*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementRectF(DomRectF*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSize(DomSize*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSizeF(DomSizeF*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementSizePolicy(DomSizePolicy*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementString(DomString*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementStringList(DomStringList*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementTime(DomTime*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setElementUrl(DomUrl*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomConnections"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomConnection"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
        ModifyFunction{
            signature: "takeElementHints()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "elementHints()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setElementHints(DomConnectionHints*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "DomConnectionHints"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomConnectionHint"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomDesignerData"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomSlots"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomPropertySpecifications"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomPropertyToolTip"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
    
    ObjectType{
        name: "DomStringPropertySpecification"
        InjectCode{
            target: CodeClass.Java
            position: Position.Comment
            Text{content: "@hidden"}
        }
    }
}
