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
    packageName: "io.qt.sql"
    defaultSuperClass: "QtObject"
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

        EnumType{
            name: "Location"
        }

        EnumType{
            name: "ParamTypeFlag"
        }

        EnumType{
            name: "TableType"
        }

        EnumType{
            name: "NumericalPrecisionPolicy"
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
    }
    
    ValueType{
        name: "QSqlQuery"

        EnumType{
            name: "BatchExecutionMode"
        }
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
    }
    
    ValueType{
        name: "QSqlError"

        EnumType{
            name: "ErrorType"
        }
    }
    
    ValueType{
        name: "QSqlIndex"
    }
    
    ValueType{
        name: "QSqlRelation"
    }
    
    ValueType{
        name: "QSqlField"

        EnumType{
            name: "RequiredStatus"
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
        EnumType{
            name: "DbmsType"
        }

        EnumType{
            name: "DriverFeature"
        }

        EnumType{
            name: "IdentifierType"
        }

        EnumType{
            name: "StatementType"
        }

        EnumType{
            name: "NotificationSource"
        }
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
            signature: "query(Qt::Disambiguated_t)const"
            ModifyArgument{
                index: 1
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "constexpr Qt::Disambiguated_t %out = Qt::Disambiguated;"}
                }
            }
            since: [6,5]
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
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }

    }
    
    ObjectType{
        name: "QSqlRelationalTableModel"
        EnumType{
            name: "JoinMode"
        }
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
        EnumType{
            name: "BindingSyntax"
        }
        EnumType{
            name: "VirtualHookOperation"
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
        ModifyFunction{
            signature: "virtual_hook(int,void*)"
            Remove{
            }
        }

        ModifyFunction{
            signature: "boundValues()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            until: 6.5
        }
        ModifyFunction{
            signature: "boundValues(Qt::Disambiguated_t)const"
            remove: RemoveFlag.All
            since: 6.6
        }
        ModifyFunction{
            signature: "boundValues(Qt::Disambiguated_t)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "constexpr Qt::Disambiguated_t %out = Qt::Disambiguated;"}
                }
            }
            since: 6.6
        }
    }
    
    ObjectType{
        name: "QSqlTableModel"

        EnumType{
            name: "EditStrategy"
        }
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
            until: [6,4]
        }
        ModifyFunction{
            signature: "beforeInsert(QSqlRecord&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "beforeUpdate(int,QSqlRecord&)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "primeInsert(int,QSqlRecord&)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
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
    
    
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QSql*."}
}
