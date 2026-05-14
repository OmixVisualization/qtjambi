/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.openapi"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtOpenApiCommon"
    module: "qtjambi.openapi"
    LoadTypeSystem{name: "QtCore"}
    NamespaceType{
        name: "QtOpenApiCommon"
        Rejection{fieldName: "isPrimitiveMediaType"}
        ModifyFunction{
            signature: "fromStringValue(QString,bool&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromStringValue(QString,double&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromStringValue(QString,float&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromStringValue(QString,qint32&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromStringValue(QString,qint64&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromByteArray(QByteArray,bool&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromByteArray(QByteArray,double&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromByteArray(QByteArray,float&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromByteArray(QByteArray,qint32&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromByteArray(QByteArray,qint64&)"
            ModifyArgument{index: 2;AsArray{}}
        }
        ModifyFunction{
            signature: "fromJsonValue(bool&,QJsonValue)"
            ModifyArgument{index: 1;AsArray{}}
        }
        ModifyFunction{
            signature: "fromJsonValue(double&,QJsonValue)"
            ModifyArgument{index: 1;AsArray{}}
        }
        ModifyFunction{
            signature: "fromJsonValue(float&,QJsonValue)"
            ModifyArgument{index: 1;AsArray{}}
        }
        ModifyFunction{
            signature: "fromJsonValue(qint32&,QJsonValue)"
            ModifyArgument{index: 1;AsArray{}}
        }
        ModifyFunction{
            signature: "fromJsonValue(qint64&,QJsonValue)"
            ModifyArgument{index: 1;AsArray{}}
        }
        ObjectType{
            name: "SerializationOptions"
        }
        NamespacePrefix{
            namingPolicy: NamespacePrefix.Cut
        }
        NamespaceType{
            name: "QOAIHttpRequestWorker"
            EnumType{
                name: "CompressionType"
            }
        }
        InterfaceType{
            name: "QOAIEnum"
        }
        ObjectType{
            name: "QOAIHttpRequestInput"
            EnumType{
                name: "VariableLayout"
            }
            ModifyFunction{
                signature: "headers()"
                remove: RemoveFlag.All
            }
        }
        ObjectType{
            name: "QOAIServerVariable"
            EnumType{
                name: "ServerError"
            }
        }
        ValueType{
            name: "QOAIHttpFileElement"
        }
        ObjectType{
            name: "QOAIBaseApi"
            Rejection{className: "QOAICallerInfo"}
            ModifyFunction{
                signature: "setRestAccessManager(QRestAccessManager*)"
                ModifyArgument{
                    index: 1
                    ReferenceCount{
                        variableName: "__rcRestAccessManager"
                        action: ReferenceCount.Set
                    }
                }
            }
        }
        ValueType{
            name: "QOAIObject"
        }
        ValueType{
            name: "QOAIServerConfiguration"
        }
        ValueType{
            name: "QOAIServerVariable"
            EnumType{
                name: "ServerError"
            }
        }
    }
}
