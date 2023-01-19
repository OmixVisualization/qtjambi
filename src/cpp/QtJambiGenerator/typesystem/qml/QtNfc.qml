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
    packageName: "io.qt.nfc"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtNfc"
    module: "qtjambi.nfc"
    description: "Provides access to Near-Field communication (NFC) hardware."

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QtNfc();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_QtNfc();"}
    }
    
    Rejection{
        className: "RequestIdPrivate"
    }
    
    EnumType{
        name: "QNearFieldShareManager::ShareError"
        until: 5
    }
    
    EnumType{
        name: "QNearFieldShareManager::ShareMode"
        flags: "QNearFieldShareManager::ShareModes"
        until: 5
    }
    
    ObjectType{
        name: "QNearFieldShareManager"
        until: 5
    }
    
    ObjectType{
        name: "QNearFieldShareTarget"
    }
    
    EnumType{
        name: "QNearFieldTarget::AccessMethod"
        flags: "QNearFieldTarget::AccessMethods"
    }
    
    EnumType{
        name: "QNearFieldTarget::Error"
    }
    
    EnumType{
        name: "QNearFieldTarget::Type"
    }
    
    ObjectType{
        name: "QNearFieldTarget"
        ModifyFunction{
            signature: "disconnect()"
            rename: "disconnectFromTarget"
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "namespace QtJambiPrivate{\n"+
                          "    template<>\n"+
                          "    struct supports_less_than<QNdefMessage> : std::false_type{};\n"+
                          "    template<>\n"+
                          "    struct supports_stream_operators<QNdefMessage> : std::false_type{};\n"+
                          "    template<>\n"+
                          "    struct supports_debugstream<QNdefMessage> : std::false_type{};\n"+
                          "}"}
        }
    }
    
    ValueType{
        name: "QNearFieldTarget::RequestId"
        ModifyFunction{
            signature: "operator=(const QNearFieldTarget::RequestId &)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QNearFieldManager::AdapterState"
    }
    
    EnumType{
        name: "QNearFieldManager::TargetAccessMode"
        flags: "QNearFieldManager::TargetAccessModes"
    }
    
    ObjectType{
        name: "QNearFieldManager"
        ModifyFunction{
            signature: "registerNdefMessageHandler(QObject *, const char *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "registerNdefMessageHandler(QNdefRecord::TypeNameFormat, const QByteArray &, QObject *, const char *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "registerNdefMessageHandler(const QNdefFilter &, QObject *, const char *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            until: 5
        }
    }
    
    ValueType{
        name: "QNdefFilter"
        ModifyFunction{
            signature: "operator=(const QNdefFilter &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "appendRecord<T>(unsigned int, unsigned int)"
            remove: RemoveFlag.All
        }
        InjectCode{
            Text{content: "@io.qt.QtUninvokable\n"+
                          "public final void appendRecord(Class<? extends QNdefRecord> type) {\n"+
                          "    appendRecord(type, 1, 1);\n"+
                          "}\n"+
                          "\n"+
                          "@io.qt.QtUninvokable\n"+
                          "public final void appendRecord(Class<? extends QNdefRecord> type, int min) {\n"+
                          "    appendRecord(type, min, 1);\n"+
                          "}\n"+
                          "\n"+
                          "@io.qt.QtUninvokable\n"+
                          "public final void appendRecord(Class<? extends QNdefRecord> type, int min, int max) {\n"+
                          "    if(type==QNdefRecord.class) {\n"+
                          "        QNdefRecord record = new QNdefRecord();\n"+
                          "        appendRecord(record.typeNameFormat(), record.type(), min, max);\n"+
                          "    }else if(type==QNdefNfcSmartPosterRecord.class) {\n"+
                          "        QNdefNfcSmartPosterRecord record = new QNdefNfcSmartPosterRecord();\n"+
                          "        appendRecord(record.typeNameFormat(), record.type(), min, max);\n"+
                          "    }else if(type==QNdefNfcTextRecord.class) {\n"+
                          "        QNdefNfcTextRecord record = new QNdefNfcTextRecord();\n"+
                          "        appendRecord(record.typeNameFormat(), record.type(), min, max);\n"+
                          "    }else if(type==QNdefNfcUriRecord.class) {\n"+
                          "        QNdefNfcUriRecord record = new QNdefNfcUriRecord();\n"+
                          "        appendRecord(record.typeNameFormat(), record.type(), min, max);\n"+
                          "    }else if(type==QNdefNfcIconRecord.class) {\n"+
                          "        QNdefNfcIconRecord record = new QNdefNfcIconRecord();\n"+
                          "        appendRecord(record.typeNameFormat(), record.type(), min, max);\n"+
                          "    }else {\n"+
                          "        try {\n"+
                          "            QNdefRecord record = type.getConstructor().newInstance();\n"+
                          "            appendRecord(record.typeNameFormat(), record.type(), min, max);\n"+
                          "        } catch (RuntimeException | Error e) {\n"+
                          "            throw e;\n"+
                          "        } catch (InstantiationException | IllegalAccessException\n"+
                          "                                        | java.lang.reflect.InvocationTargetException | NoSuchMethodException e) {\n"+
                          "            throw new RuntimeException(e);\n"+
                          "        }\n"+
                          "    }\n"+
                          "}"}
        }
    }
    
    ValueType{
        name: "QNdefFilter::Record"
    }
    
    ValueType{
        name: "QNdefMessage"
        ModifyFunction{
            signature: "operator=(const QNdefMessage &)"
            remove: RemoveFlag.All
            since: [6, 2]
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "namespace QtJambiPrivate{\n"+
                          "    template<>\n"+
                          "    struct supports_less_than<QNdefMessage> : std::false_type{};\n"+
                          "    template<>\n"+
                          "    struct supports_stream_operators<QNdefMessage> : std::false_type{};\n"+
                          "    template<>\n"+
                          "    struct supports_debugstream<QNdefMessage> : std::false_type{};\n"+
                          "}"}
        }
    }
    
    EnumType{
        name: "QNdefNfcSmartPosterRecord::Action"
    }
    
    ValueType{
        name: "QNdefNfcSmartPosterRecord"
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcSmartPosterRecord>()"
        ModifyFunction{
            signature: "operator=(const QNdefNfcSmartPosterRecord &)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QNdefNfcTextRecord::Encoding"
    }
    
    ValueType{
        name: "QNdefNfcTextRecord"
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcTextRecord>()"
        ModifyFunction{
            signature: "operator=(const QNdefNfcTextRecord &)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ValueType{
        name: "QNdefNfcUriRecord"
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcUriRecord>()"
    }
    
    ValueType{
        name: "QNdefNfcIconRecord"
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcIconRecord>()"
    }
    
    EnumType{
        name: "QNdefRecord::TypeNameFormat"
    }
    
    ValueType{
        name: "QNdefRecord"
        isPolymorphicBase: true
        polymorphicIdExpression: "%1->isRecordType%lt;QNdefRecord>()"
        ModifyFunction{
            signature: "operator=(const QNdefRecord &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isRecordType<T>() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setPayload(const QByteArray &)"
            access: Modification.NonFinal
        }
    }
    
    EnumType{
        name: "QQmlNdefRecord::TypeNameFormat"
        until: 5
    }
    
    ObjectType{
        name: "QQmlNdefRecord"
        until: 5
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNdefFilter::Record."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNdefFilter."}
}
