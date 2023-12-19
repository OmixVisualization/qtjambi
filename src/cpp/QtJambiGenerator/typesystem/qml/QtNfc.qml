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
    defaultSuperClass: "QtObject"
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

    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.Optional
        until: 5
    }
    
    Rejection{
        className: "RequestIdPrivate"
    }
    
    ObjectType{
        name: "QNearFieldShareManager"

        EnumType{
            name: "ShareError"
        }

        EnumType{
            name: "ShareMode"
        }
        until: 5
    }
    
    ObjectType{
        name: "QNearFieldShareTarget"
    }
    
    ObjectType{
        name: "QNearFieldTarget"

        EnumType{
            name: "AccessMethod"
        }

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "Type"
        }

        ValueType{
            name: "RequestId"
        }
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
    
    ObjectType{
        name: "QNearFieldManager"

        EnumType{
            name: "AdapterState"
        }

        EnumType{
            name: "TargetAccessMode"
        }
    }
    
    ValueType{
        name: "QNdefFilter"

        ValueType{
            name: "Record"
        }
        ModifyFunction{
            signature: "appendRecord<T>(unsigned int, unsigned int)"
            remove: RemoveFlag.All
        }
        InjectCode{
            Text{content: "@QtUninvokable\n"+
                          "public final void appendRecord(Class<? extends QNdefRecord> type) {\n"+
                          "    appendRecord(type, 1, 1);\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public final void appendRecord(Class<? extends QNdefRecord> type, int min) {\n"+
                          "    appendRecord(type, min, 1);\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
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
        name: "QNdefMessage"
        noImplicitConstructors: true
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
                          "}\n"+
                          "QT_WARNING_DISABLE_CLANG(\"-Wdeprecated-copy\")\n"+
                          "QT_WARNING_DISABLE_GCC(\"-Wdeprecated-copy\")"}
        }
    }
    
    ValueType{
        name: "QNdefNfcSmartPosterRecord"

        EnumType{
            name: "Action"
        }
        ModifyFunction{
            signature: "QNdefNfcSmartPosterRecord(QNdefRecord)"
            isForcedExplicit: true
        }
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcSmartPosterRecord>()"
    }
    
    ValueType{
        name: "QNdefNfcTextRecord"
        noImplicitConstructors: true
        EnumType{
            name: "Encoding"
        }
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcTextRecord>()"
    }
    
    ValueType{
        name: "QNdefNfcUriRecord"
        noImplicitConstructors: true
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcUriRecord>()"
    }
    
    ValueType{
        name: "QNdefNfcIconRecord"
        noImplicitConstructors: true
        polymorphicIdExpression: "%1->isRecordType<QNdefNfcIconRecord>()"
    }
    
    ValueType{
        name: "QNdefRecord"

        EnumType{
            name: "TypeNameFormat"
        }
        isPolymorphicBase: true
        polymorphicIdExpression: "%1->isRecordType%lt;QNdefRecord>()"
        ModifyFunction{
            signature: "isRecordType<T>() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setPayload(const QByteArray &)"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QQmlNdefRecord"

        EnumType{
            name: "TypeNameFormat"
        }
        until: 5
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNdefFilter::Record."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNdefFilter."}
}
