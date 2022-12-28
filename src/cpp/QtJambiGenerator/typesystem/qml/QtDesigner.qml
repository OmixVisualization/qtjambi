/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.designer"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtDesigner"
    module: "qtjambi.designer"
    description: "Classes for loading QWidget based forms created in Qt Designer dynamically, at runtime."
    InjectCode{
        target: CodeClass.ModuleInfo
        Text{content: "exports io.qt.designer.util;"}
    }
    
    PrimitiveType{
        name: "qdesigner_internal::FlagType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "qdesigner_internal::EnumType"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    RequiredLibrary{
        name: "QtPrintSupport"
        mode: RequiredLibrary.ProvideOnly
    }
    
    RequiredLibrary{
        name: "QtWebEngineCore"
        mode: RequiredLibrary.ProvideOnly
    }
    
    RequiredLibrary{
        name: "QtWebEngineWidgets"
        mode: RequiredLibrary.ProvideOnly
    }
    
    Rejection{
        className: ""
        functionName: "qt_extension"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "toString"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "propertyMap"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "domPixmap"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "addItem"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "applyProperties"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "applyTabStops"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "computeProperties"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "create"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "createActionRefDom"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "createConnections"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "createCustomWidgets"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "createDom"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "createProperty"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "createResources"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "initialize"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "layoutInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadButtonExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadComboBoxExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadItemViewExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadListWidgetExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadTreeWidgetExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "loadTableWidgetExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveButtonExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveComboBoxExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveItemViewExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveListWidgetExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveTreeWidgetExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveTableWidgetExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveBrush"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveButtonExtraInfo"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveButtonGroups"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveColorGroup"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveConnections"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveCustomWidgets"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveDom"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveResources"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveTabStops"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveResource"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "saveText"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "toVariant"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "setIconProperty"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "setPixmapProperty"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "setupBrush"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "setupColorGroup"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "domPropertyToIcon"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "domPropertyToPixmap"
    }
    
    Rejection{
        className: "QAbstractFormBuilder"
        functionName: "iconToDomProperty"
    }
    
    Rejection{
        className: "QFormBuilder"
        functionName: "addItem"
    }
    
    Rejection{
        className: "QFormBuilder"
        functionName: "applyProperties"
    }
    
    Rejection{
        className: "QFormBuilder"
        functionName: "create"
    }
    
    Rejection{
        className: "QFormBuilder"
        functionName: "createConnections"
    }
    
    Rejection{
        className: "QDesignerFormWindowToolInterface"
        functionName: "loadFromDom"
    }
    
    Rejection{
        className: "QDesignerFormWindowToolInterface"
        functionName: "saveToDom"
    }
    
    Rejection{
        className: "QDesignerFormWindowInterface"
        functionName: "resourceSet"
    }
    
    Rejection{
        className: "QDesignerFormWindowInterface"
        functionName: "setResourceSet"
    }
    
    Rejection{
        className: "QDesignerFormWindowManagerInterface"
        enumName: "Action"
    }
    
    Rejection{
        className: "QDesignerFormWindowManagerInterface"
        enumName: "ActionGroup"
    }
    
    Rejection{
        className: "QDesignerFormWindowManagerInterface"
    }
    
    Rejection{
        className: "QtResourceSet"
    }
    
    Rejection{
        className: "QtGradientManager"
    }
    
    Rejection{
        className: "QDesignerPluginManager"
    }
    
    Rejection{
        className: "QDesignerIntrospectionInterface"
    }
    
    Rejection{
        className: "QDesignerDialogGuiInterface"
    }
    
    Rejection{
        className: "QtResourceModel"
    }
    
    Rejection{
        className: "QTextBuilder"
    }
    
    Rejection{
        className: "QAbstractFormBuilder::DomPropertyHash"
    }
    
    Rejection{
        className: "QDesignerDnDItemInterface"
    }
    
    Rejection{
        className: "QDesignerExtraInfoExtension"
    }
    
    Rejection{
        className: "QResourceBuilder"
    }
    
    Rejection{
        className: "QDesignerDnDItemInterface"
    }
    
    InterfaceType{
        name: "QDesignerLanguageExtension"
        ModifyFunction{
            signature: "createFormWindowSettingsDialog(QDesignerFormWindowInterface*,QWidget*)"
            ModifyArgument{
                index: "return"
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
            signature: "createPromotionDialog(QDesignerFormEditorInterface*,QWidget*)"
            ModifyArgument{
                index: "return"
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
            signature: "createPromotionDialog(QDesignerFormEditorInterface*,QString,QString*,QWidget*)"
            ModifyArgument{
                index: 3
                ArrayType{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: "return"
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
            signature: "createResourceBrowser(QWidget*)"
            ModifyArgument{
                index: "return"
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
    }
    
    InterfaceType{
        name: "QDesignerFormEditorPluginInterface"
    }
    
    ObjectType{
        name: "QDesignerFormEditorInterface"
        ModifyFunction{
            signature: "setActionEditor(QDesignerActionEditorInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActionEditor"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setExtensionManager(QExtensionManager*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcExtensionManager"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setIntegration(QDesignerIntegrationInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIntegration"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setMetaDataBase(QDesignerMetaDataBaseInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMetaDataBase"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setObjectInspector(QDesignerObjectInspectorInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObjectInspector"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setPromotion(QDesignerPromotionInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPromotion"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setPropertyEditor(QDesignerPropertyEditorInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPropertyEditor"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSettingsManager(QDesignerSettingsInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSettingsManager"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setWidgetDataBase(QDesignerWidgetDataBaseInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidgetDataBase"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setWidgetFactory(QDesignerWidgetFactoryInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidgetFactory"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setWidgetBox(QDesignerWidgetBoxInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidgetBox"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTopLevel(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QDesignerWidgetBoxInterface"
    }
    
    EnumType{
        name: "QDesignerWidgetBoxInterface::Category::Type"
    }
    
    ValueType{
        name: "QDesignerWidgetBoxInterface::Category"
    }
    
    EnumType{
        name: "QDesignerWidgetBoxInterface::Widget::Type"
    }
    
    ValueType{
        name: "QDesignerWidgetBoxInterface::Widget"
        ModifyFunction{
            signature: "operator=(QDesignerWidgetBoxInterface::Widget)"
            remove: RemoveFlag.All
        }
    }
    
    InterfaceType{
        name: "QAbstractExtensionFactory"
    }
    
    InterfaceType{
        name: "QAbstractExtensionManager"
        ModifyFunction{
            signature: "registerExtensions(QAbstractExtensionFactory *, QString)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcExtension"
                    action: ReferenceCount.Add
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "unregisterExtensions(QAbstractExtensionFactory *, QString)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcExtension"
                    action: ReferenceCount.Take
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Default
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractFormBuilder"
        ExtraIncludes{
            Include{
                fileName: "QtDesigner/private/ui4_p.h"
                suppressed: true
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "addMenuAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "load(QIODevice *, QWidget *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createAction(QObject*,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createActionGroup(QObject*,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createLayout(QString,QObject*,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createWidget(QString,QWidget*,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QDesignerActionEditorInterface"
        ModifyFunction{
            signature: "setFormWindow(QDesignerFormWindowInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFormWindow"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerBrushManagerInterface"
    }
    
    InterfaceType{
        name: "QDesignerContainerExtension"
        ModifyFunction{
            signature: "addWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertWidget(int,QWidget*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerCustomWidgetCollectionInterface"
        ExtraIncludes{
            Include{
                fileName: "customwidget.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "QtUiPlugin/customwidget.h"
                location: Include.Global
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerCustomWidgetInterface"
        ExtraIncludes{
            Include{
                fileName: "customwidget.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "QtUiPlugin/customwidget.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "createWidget(QWidget*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    EnumType{
        name: "QDesignerFormWindowCursorInterface::MoveOperation"
    }
    
    EnumType{
        name: "QDesignerFormWindowCursorInterface::MoveMode"
    }
    
    InterfaceType{
        name: "QDesignerFormWindowCursorInterface"
        ModifyFunction{
            signature: "setWidgetProperty(QWidget*,QString,QVariant)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QDesignerFormWindowInterface"
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.End
            Text{content: "public:\n"+
                          "    inline QtResourceSet *resourceSet() const override {return m_resourceSet;}\n"+
                          "    inline void setResourceSet(QtResourceSet *resourceSet) override { m_resourceSet = resourceSet; }\n"+
                          "private:\n"+
                          "    QtResourceSet * m_resourceSet = nullptr;"}
        }
        ModifyFunction{
            signature: "cursor() const"
            rename: "cursorInterface"
        }
        ModifyFunction{
            signature: "registerTool(QDesignerFormWindowToolInterface *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "setContents(QIODevice*,QString*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ArrayType{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "setMainContainer(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMainContainer"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "layoutDefault(int*,int*)"
            ModifyArgument{
                index: 1
                ArrayType{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 2
                ArrayType{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "layoutFunction(QString*,QString*)"
            ModifyArgument{
                index: 1
                ArrayType{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 2
                ArrayType{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "activateResourceFilePaths(QStringList,int*,QString*)"
            ModifyArgument{
                index: 2
                ArrayType{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 3
                ArrayType{
                    minLength: 1
                }
            }
        }
    }
    
    EnumType{
        name: "QDesignerFormWindowInterface::FeatureFlag"
        flags: "QDesignerFormWindowInterface::Feature"
    }
    
    EnumType{
        name: "QDesignerFormWindowInterface::ResourceFileSaveMode"
    }
    
    EnumType{
        name: "QDesignerIntegrationInterface::ResourceFileWatcherBehaviour"
    }
    
    EnumType{
        name: "QDesignerIntegrationInterface::FeatureFlag"
        flags: "QDesignerIntegrationInterface::Feature"
    }
    
    ObjectType{
        name: "QDesignerFormWindowToolInterface"
        ModifyFunction{
            signature: "handleEvent(QWidget*,QWidget*,QEvent*)"
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerIconCacheInterface"
    }
    
    ObjectType{
        name: "QDesignerIntegrationInterface"
        ModifyFunction{
            signature: "createResourceBrowser(QWidget*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setupFormWindow(QDesignerFormWindowInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    EnumType{
        name: "QDesignerLayoutDecorationExtension::InsertMode"
    }
    
    InterfaceType{
        name: "QDesignerLayoutDecorationExtension"
        ModifyFunction{
            signature: "insertWidget(QWidget*,std::pair<int,int>)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidgets"
                    action: ReferenceCount.Add
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "insertWidget(QWidget*,QPair<int,int>)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidgets"
                    action: ReferenceCount.Add
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "removeWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidgets"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "indexOf(QLayoutItem*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QDesignerMetaDataBaseInterface"
        ModifyFunction{
            signature: "add(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObjects"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "remove(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObjects"
                    action: ReferenceCount.Take
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerMetaDataBaseItemInterface"
    }
    
    ObjectType{
        name: "QDesignerObjectInspectorInterface"
        ModifyFunction{
            signature: "setFormWindow(QDesignerFormWindowInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFormWindow"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QDesignerPropertyEditorInterface"
        ModifyFunction{
            signature: "setObject(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObject"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerTaskMenuExtension"
    }
    
    ObjectType{
        name: "QDesignerWidgetDataBaseInterface"
        ModifyFunction{
            signature: "insert(int,QDesignerWidgetDataBaseItemInterface*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "append(QDesignerWidgetDataBaseItemInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "indexOf(QDesignerWidgetDataBaseItemInterface*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerWidgetDataBaseItemInterface"
        ModifyFunction{
            signature: "extends() const"
            rename: "getExtends"
        }
    }
    
    ObjectType{
        name: "QDesignerWidgetFactoryInterface"
        ModifyFunction{
            signature: "createLayout(QWidget*,QLayout*,int)const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createWidget(QString,QWidget*)const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QExtensionFactory"
        ModifyFunction{
            signature: "createExtension(QObject *, QString, QObject *) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QExtensionManager"
        ModifyFunction{
            signature: "registerExtensions(QAbstractExtensionFactory *, QString)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcExtension"
                    action: ReferenceCount.Add
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "unregisterExtensions(QAbstractExtensionFactory *, QString)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcExtension"
                    action: ReferenceCount.Take
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Default
                }
            }
        }
    }
    
    ObjectType{
        name: "QFormBuilder"
        ExtraIncludes{
            Include{
                fileName: "QtDesigner/private/ui4_p.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "customwidget.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "QtUiPlugin/customwidget.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "createLayout(QString,QObject*,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createWidget(QString,QWidget*,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerDynamicPropertySheetExtension"
    }
    
    InterfaceType{
        name: "QDesignerPromotionInterface"
        ModifyFunction{
            signature: "addPromotedClass(QString,QString,QString,QString*)"
            ModifyArgument{
                index: 4
                ArrayType{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "changePromotedClassName(QString,QString,QString*)"
            ModifyArgument{
                index: 3
                ArrayType{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "setPromotedClassIncludeFile(QString,QString,QString*)"
            ModifyArgument{
                index: 3
                ArrayType{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "removePromotedClass(QString,QString*)"
            ModifyArgument{
                index: 2
                ArrayType{
                    minLength: 1
                }
            }
        }
    }
    
    ValueType{
        name: "QDesignerPromotionInterface::PromotedClass"
        ExtraIncludes{
            Include{
                fileName: "QtDesigner/QDesignerWidgetDataBaseItemInterface"
                location: Include.Global
            }
        }
        ModifyField{
            name: "baseItem"
            ReferenceCount{
                action: ReferenceCount.Ignore
            }
        }
        ModifyField{
            name: "promotedItem"
            ReferenceCount{
                action: ReferenceCount.Ignore
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerScriptExtension"
    }
    
    InterfaceType{
        name: "QDesignerPropertySheetExtension"
    }
    
    InterfaceType{
        name: "QDesignerMemberSheetExtension"
    }
    
    ObjectType{
        name: "QDesignerIntegration"
        ModifyFunction{
            signature: "createResourceBrowser(QWidget*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setupFormWindow(QDesignerFormWindowInterface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QDesignerNewFormWidgetInterface"
        ModifyFunction{
            signature: "createNewFormWidget(QDesignerFormEditorInterface*,QWidget*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "currentTemplate(QString*)"
            ModifyArgument{
                index: 1
                ArrayType{
                    minLength: 1
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerOptionsPageInterface"
        ModifyFunction{
            signature: "createPage(QWidget*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    InterfaceType{
        name: "QDesignerSettingsInterface"
    }
    
    ObjectType{
        name: "QDesignerResourceBrowserInterface"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QtResourceModel*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerIntrospectionInterface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QtGradientManager*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerPluginManager*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QtResourceSet*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QResourceBuilder*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QTextBuilder*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QAbstractFormBuilder::DomPropertyHash*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerDialogGuiInterface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerFormWindowManagerInterface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerWidgetBoxInterface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerLanguageExtension*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QDesignerFormEditorPluginInterface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*Dom*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QList<Dom*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QList<QDesignerDnDItemInterface*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QDesigner*::*."}
}
