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
    packageName: "io.qt.activex"
    defaultSuperClass: "QtObject"
    targetName: "QtJambiActiveX"
    module: "qtjambi.activex"
    description: "Classes for applications which use ActiveX and COM"
    defaultPPCondition: "defined(Q_OS_WIN)"
    ExtraIncludes{
        Include{
            fileName: "qaxobject.h"
            location: Include.Global
            ckeckAvailability: true
        }
        Include{
            fileName: "qaxwidget.h"
            location: Include.Global
            ckeckAvailability: true
        }
        Include{
            fileName: "QtJambi/RegistryAPI"
            location: Include.Global
        }
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.End
        Text{content: "#if defined(Q_OS_WIN)\n"+
                      "RegistryAPI::registerMetaType<HRESULT>(\"HRESULT\");\n"+
                      "RegistryAPI::registerPrimitiveTypeInfo<HRESULT>(\"HRESULT\", \"int\");\n"+
                      "#endif"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.End
        until: [5, 15]
        Text{content: "#if defined(Q_OS_WIN)\n"+
                      "RegistryAPI::registerMetaObject(typeid(QAxObject), QAxObject::staticMetaObject, false);\n"+
                      "RegistryAPI::registerMetaObject(typeid(QAxWidget), QAxWidget::staticMetaObject, false);\n"+
                      "#endif"}
    }
    
    RequiredLibrary{
        name: "QtGui"
    }
    
    RequiredLibrary{
        name: "QtWidgets"
    }
    
    Rejection{
        className: "IUnknown"
    }
    
    Rejection{
        className: "QAxClass"
    }
    
    Rejection{
        className: "QAxFactory"
    }
    
    Rejection{
        className: "QAxAggregated"
    }
    
    Rejection{
        className: "QAxBindable"
    }
    
    Rejection{
        className: "QAxMetaObject"
    }
    
    Rejection{
        className: "IAxServerBase"
    }
    
    Rejection{
        className: ""
        functionName: "qAxFactory"
    }
    
    Rejection{
        className: ""
        functionName: "qax_create_object_wrapper"
    }
    
    Rejection{
        className: ""
        functionName: "qax_generateDocumentation"
    }
    
    Rejection{
        className: ""
        functionName: "qax_startServer"
    }
    
    Rejection{
        className: ""
        functionName: "qax_stopServer"
    }
    
    Rejection{
        className: ""
        functionName: "QVariantToVARIANT"
    }
    
    Rejection{
        className: ""
        functionName: "QVariantToVARIANT_container"
    }
    
    Rejection{
        className: ""
        functionName: "QVariantToVoidStar"
    }
    
    Rejection{
        className: ""
        functionName: "VARIANTToQVariant"
    }
    
    Rejection{
        className: ""
        functionName: "VARIANTToQVariant_container"
    }
    
    Rejection{
        className: ""
        functionName: "clearVARIANT"
    }
    
    Rejection{
        className: ""
        functionName: "OLEColorToQColor"
    }
    
    Rejection{
        className: ""
        functionName: "QColorToOLEColor"
    }
    
    Rejection{
        className: ""
        functionName: "QStringToBSTR"
    }
    
    ObjectType{
        name: "QAxBaseObject"

        Rejection{
            functionName: "queryInterface"
        }
        ModifyFunction{
            signature: "signal(QString, int, void *)"
            access: Modification.Private
        }
        since: 6
    }
    
    ObjectType{
        name: "QAxBaseWidget"

        Rejection{
            functionName: "queryInterface"
        }
        ModifyFunction{
            signature: "signal(QString, int, void *)"
            access: Modification.Private
        }
        since: 6
    }
    
    ObjectType{
        name: "QAxWidget"

        Rejection{
            functionName: "queryInterface"
        }
        ModifyFunction{
            signature: "qt_static_metacall(QObject *, QMetaObject::Call, int, void **)"
            remove: RemoveFlag.All
            since: 6
        }
        InjectCode{
            until: [5, 15]
            Text{content: "public static final io.qt.core.QMetaObject staticMetaObject = io.qt.core.QMetaObject.forType(QAxWidget.class);"}
        }
    }
    
    InterfaceType{
        name: "QAxObjectInterface"
    }
    
    InterfaceType{
        name: "QAxBase"

        Rejection{
            className: "qt_meta_stringdata_QAxBase_t"
        }
        Rejection{
            fieldName: "qt_meta_data_QAxBase"
        }
        Rejection{
            fieldName: "qt_meta_stringdata_QAxBase"
        }
        Rejection{
            functionName: "queryInterface"
        }
        Rejection{
            functionName: "qt_metacall"
        }
        Rejection{
            functionName: "qt_static_metacall"
        }
    }
    
    ObjectType{
        name: "QAxObject"

        Rejection{
            functionName: "queryInterface"
        }
        ModifyFunction{
            signature: "qt_static_metacall(QObject *, QMetaObject::Call, int, void **)"
            remove: RemoveFlag.All
            since: 6
        }
        InjectCode{
            until: [5, 15]
            Text{content: "public static final io.qt.core.QMetaObject staticMetaObject = io.qt.core.QMetaObject.forType(QAxObject.class);"}
        }
    }
    
    ObjectType{
        name: "QAxScript"

        EnumType{
            name: "FunctionFlags"
        }
    }
    
    ObjectType{
        name: "QAxScriptEngine"

        Rejection{
            functionName: "queryInterface"
        }

        EnumType{
            name: "State"
        }
    }
    
    ObjectType{
        name: "QAxScriptManager"
        ModifyFunction{
            signature: "addObject(QObject *)"
            remove: RemoveFlag.All
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObjects"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addObject(QAxBase *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObjects"
                    action: ReferenceCount.Add
                }
            }
        }
    }
    
    ObjectType{
        name: "QAxSelect"

        EnumType{
            name: "SandboxingLevel"
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'IUnknown*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'QAxAggregated*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'QAxMetaObject*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'metaObject() const' in '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'qt_metacall(*)' in '*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcObjects' in 'QAxScriptManager' but not both."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping field 'IID_IAxServerBase' with unmatched type 'GUID'"}
}
