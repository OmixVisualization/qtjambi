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
    packageName: "io.qt.qt3d.input"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DInput"
    module: "qtjambi.qt3dinput"
    description: "The Qt 3D Input module provides classes for handling user input in applications using Qt3D."

    NamespacePrefix{
        prefix: "Qt3DInput"
        namespace: "Qt3DInput"
        namingPolicy: NamespacePrefix.Cut
    }
    
    NamespaceType{
        name: "Qt3DInput"
        generate: false
        Rejection{
            className: "QPhysicalDeviceCreatedChange"
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QButtonAxisInput"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QAbstractActionInput"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QAbstractAxisInput"
        ModifyFunction{
            signature: "setSourceDevice(Qt3DInput::QAbstractPhysicalDevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QKeyboardDevice"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QMouseDevice"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QAxisAccumulator"
        EnumType{
            name: "SourceAxisType"
        }
        ModifyFunction{
            signature: "setSourceAxis(Qt3DInput::QAxis*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }        
        since: [5, 8]
    }
    
    ObjectType{
        name: "Qt3DInput::QMouseHandler"
        EnumType{
            name: "Axis"
        }
        ModifyFunction{
            signature: "wheel(Qt3DInput::QWheelEvent *)"
            ppCondition: "QT_CONFIG(wheelevent)"
        }
        ModifyFunction{
            signature: "setSourceDevice(Qt3DInput::QMouseDevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 7]
    }

    ObjectType{
        name: "Qt3DInput::QKeyboardHandler"
        ModifyFunction{
            signature: "setSourceDevice(Qt3DInput::QKeyboardDevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QAnalogAxisInput"
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QInputChord"
        ModifyFunction{
            signature: "addChord(Qt3DInput::QAbstractActionInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        ModifyFunction{
            signature: "removeChord(Qt3DInput::QAbstractActionInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QInputSequence"
        ModifyFunction{
            signature: "addSequence(Qt3DInput::QAbstractActionInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        ModifyFunction{
            signature: "removeSequence(Qt3DInput::QAbstractActionInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DInput::QAbstractPhysicalDevice"
        ModifyFunction{
            signature: "addAxisSetting(Qt3DInput::QAxisSetting*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeAxisSetting(Qt3DInput::QAxisSetting*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QAbstractPhysicalDeviceBackendNode"
    }
    
    ObjectType{
        name: "Qt3DInput::QAction"
        ModifyFunction{
            signature: "addInput(Qt3DInput::QAbstractActionInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        ModifyFunction{
            signature: "removeInput(Qt3DInput::QAbstractActionInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QInputSettings"
        ModifyFunction{
            signature: "setEventSource(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcEventSource"
                    action: ReferenceCount.Set
                }
            }
            since: [5, 7]
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QActionInput"
        ModifyFunction{
            signature: "setSourceDevice(Qt3DInput::QAbstractPhysicalDevice *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSourceDevice"
                    action: ReferenceCount.Set
                }
            }
            since: [5, 7]
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QAxis"
        ModifyFunction{
            signature: "addInput(Qt3DInput::QAbstractAxisInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
        ModifyFunction{
            signature: "removeInput(Qt3DInput::QAbstractAxisInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [5, 7]
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QAxisActionHandler"
    }
    
    ObjectType{
        name: "Qt3DInput::QAxisInput"
    }
    
    ObjectType{
        name: "Qt3DInput::QAxisSetting"
    }
    
    ObjectType{
        name: "Qt3DInput::QInputDeviceIntegration"
        ModifyFunction{
            signature: "registerBackendType(const QSharedPointer<Qt3DCore::QBackendNodeFunctor> &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "registerBackendType(const QMetaObject &, const QSharedPointer<Qt3DCore::QBackendNodeFunctor> &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QInputDevicePlugin"
    }
    
    ObjectType{
        name: "Qt3DInput::QMouseEvent"
        EnumType{
            name: "Buttons"
        }

        EnumType{
            name: "Modifiers"
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QWheelEvent"
        EnumType{
            name: "Buttons"
            ppCondition: "QT_CONFIG(wheelevent)"
        }

        EnumType{
            name: "Modifiers"
            ppCondition: "QT_CONFIG(wheelevent)"
        }
        ppCondition: "QT_CONFIG(wheelevent)"
    }
    
    ObjectType{
        name: "Qt3DInput::QMouseInput"
    }
    
    ObjectType{
        name: "Qt3DInput::QKeyEvent"
        ModifyFunction{
            signature: "matches(QKeySequence::StandardKey) const"
            ppCondition: "QT_CONFIG(shortcut)"
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QLogicalDevice"
        ModifyFunction{
            signature: "addAxis(Qt3DInput::QAxis*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeAxis(Qt3DInput::QAxis*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addAction(Qt3DInput::QAction*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeAction(Qt3DInput::QAction*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QMouseController"
        EnumType{
            name: "Axis"
        }

        EnumType{
            name: "Button"
        }
        ModifyFunction{
            signature: "axisCount() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "buttonCount() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "axisNames() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "buttonNames() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "axisIdentifier(const QString &) const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "buttonIdentifier(const QString &) const"
            access: Modification.DeclFinal
        }
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DInput::QInputAspect"
        ModifyFunction{
            signature: "setCamera(Qt3DRender::QCamera*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcCamera"
                    action: ReferenceCount.Set
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "createPhysicalDevice(const QString &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QKeyboardController"
        ModifyFunction{
            signature: "axisCount() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "buttonCount() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "axisNames() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "buttonNames() const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "axisIdentifier(const QString &) const"
            access: Modification.DeclFinal
        }
        ModifyFunction{
            signature: "buttonIdentifier(const QString &) const"
            access: Modification.DeclFinal
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QKeyboardInput"
        ModifyFunction{
            signature: "setController(Qt3DInput::QKeyboardController*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DInput::QPhysicalDeviceCreatedChangeBase"
        since: [5, 7]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*::doClone', unmatched return type 'QNode*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractActionInput' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractAxisInput' set to non-final, as it is extended by other classes"}
}
