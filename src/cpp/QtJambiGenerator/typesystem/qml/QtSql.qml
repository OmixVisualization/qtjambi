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
    packageName: "io.qt.sql"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtSql"
    module: "qtjambi.sql"
    description: "Classes for database integration using SQL."
    RequiredPackage{
        name: "io.qt.gui"
    }
    
    RequiredPackage{
        name: "io.qt.widgets"
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position2
        Text{content: "void initialize_meta_info_QSqlRelationalDelegate();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Beginning
        Text{content: "initialize_meta_info_QSqlRelationalDelegate();"}
    }
    
    Rejection{
        className: "QSqlRelationalDelegate"
    }
    
    Rejection{
        className: "QSqlDriverCreator"
    }
    
    Rejection{
        className: "QSqlError::Unused"
    }
    
    NamespaceType{
        name: "QSql"
        Include{
            fileName: "qtsqlglobal.h"
            location: Include.Global
        }
    }
    
    ObjectType{
        name: "QSqlDriverPlugin"
        ModifyFunction{
            signature: "create(QString)"
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
        InjectCode{
            target: CodeClass.MetaInfo
            position: Position.Position1
            Text{content: "registerInterfaceID(typeId, QSqlDriverFactoryInterface_iid);"}
        }
    }
    
    InterfaceType{
        name: "QSqlDriverFactoryInterface"
    }
    
    ValueType{
        name: "QSqlDatabase"
        ExtraIncludes{
            Include{
                fileName: "QSqlQuery"
                location: Include.Global
            }
            Include{
                fileName: "QSqlError"
                location: Include.Global
            }
            Include{
                fileName: "QSqlIndex"
                location: Include.Global
            }
            Include{
                fileName: "QSqlRecord"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "operator=(QSqlDatabase)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "registerSqlDriver(QString, QSqlDriverCreatorBase *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addDatabase(QSqlDriver*,QString)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "contains(QString)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "defaultConnection()"
                }
            }
        }
        ModifyFunction{
            signature: "addDatabase(QString,QString)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "defaultConnection()"
                }
            }
        }
        ModifyFunction{
            signature: "addDatabase(QSqlDriver *,QString)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "defaultConnection()"
                }
            }
        }
        ModifyFunction{
            signature: "database(QString,bool)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "defaultConnection()"
                }
            }
        }
        ModifyField{
            name: "defaultConnection"
            ReplaceType{
                modifiedType: "java.lang.String"
            }
        }
    }
    
    ValueType{
        name: "QSqlQuery"
        ExtraIncludes{
            Include{
                fileName: "QSqlError"
                location: Include.Global
            }
            Include{
                fileName: "QSqlRecord"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "operator=(QSqlQuery)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QSqlRecord"
        ExtraIncludes{
            Include{
                fileName: "QSqlField"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "append(QSqlField)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "operator=(QSqlRecord)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QSqlError"
        ModifyFunction{
            signature: "operator=(QSqlError)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QSqlIndex"
        ModifyFunction{
            signature: "operator=(QSqlIndex)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QSqlRelation"
    }
    
    ValueType{
        name: "QSqlField"
        ModifyFunction{
            signature: "operator=(QSqlField)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QSqlField(QString,QVariant::Type)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "io.qt.core.QVariant.Type.Invalid"
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QSqlDriver"
        ExtraIncludes{
            Include{
                fileName: "QSqlQuery"
                location: Include.Global
            }
            Include{
                fileName: "QSqlError"
                location: Include.Global
            }
            Include{
                fileName: "QSqlIndex"
                location: Include.Global
            }
            Include{
                fileName: "QSqlRecord"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "createResult()const"
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
    }
    
    ObjectType{
        name: "QSqlQueryModel"
        ModifyFunction{
            signature: "record()const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "record(int)const"
            access: Modification.NonFinal
        }
        ExtraIncludes{
            Include{
                fileName: "QSqlError"
                location: Include.Global
            }
            Include{
                fileName: "QSqlQuery"
                location: Include.Global
            }
            Include{
                fileName: "QSqlRecord"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QSqlRelationalTableModel"
        ExtraIncludes{
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QSqlResult"
        ExtraIncludes{
            Include{
                fileName: "QSqlError"
                location: Include.Global
            }
            Include{
                fileName: "QSqlQuery"
                location: Include.Global
            }
            Include{
                fileName: "QSqlRecord"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "virtual_hook(int,void*)"
            Remove{
            }
        }
    }
    
    ObjectType{
        name: "QSqlTableModel"
        ExtraIncludes{
            Include{
                fileName: "QSqlIndex"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setQuery(QSqlQuery)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "beforeInsert(QSqlRecord&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "io.qt.sql.QSqlRecord"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, &%in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSqlRecord &%out = qtjambi_cast<QSqlRecord &>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "beforeUpdate(int,QSqlRecord&)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "io.qt.sql.QSqlRecord"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, &%in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSqlRecord &%out = qtjambi_cast<QSqlRecord &>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "primeInsert(int,QSqlRecord&)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "io.qt.sql.QSqlRecord"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, &%in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSqlRecord &%out = qtjambi_cast<QSqlRecord &>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QSqlDriverCreatorBase"
        ExtraIncludes{
            Include{
                fileName: "QSqlDriver"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "createObject()const"
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
    }
    
    EnumType{
        name: "QSql::Location"
    }
    
    EnumType{
        name: "QSql::ParamTypeFlag"
        flags: "QSql::ParamType"
    }
    
    EnumType{
        name: "QSql::TableType"
    }
    
    EnumType{
        name: "QSql::NumericalPrecisionPolicy"
    }
    
    EnumType{
        name: "QSqlDriver::DbmsType"
    }
    
    EnumType{
        name: "QSqlDriver::DriverFeature"
    }
    
    EnumType{
        name: "QSqlDriver::IdentifierType"
    }
    
    EnumType{
        name: "QSqlDriver::StatementType"
    }
    
    EnumType{
        name: "QSqlError::ErrorType"
    }
    
    EnumType{
        name: "QSqlField::RequiredStatus"
    }
    
    EnumType{
        name: "QSqlQuery::BatchExecutionMode"
    }
    
    EnumType{
        name: "QSqlResult::BindingSyntax"
    }
    
    EnumType{
        name: "QSqlResult::VirtualHookOperation"
    }
    
    EnumType{
        name: "QSqlTableModel::EditStrategy"
    }
    
    EnumType{
        name: "QSqlDriver::NotificationSource"
    }
    
    EnumType{
        name: "QSqlRelationalTableModel::JoinMode"
    }
    
    
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QSql*."}
}
