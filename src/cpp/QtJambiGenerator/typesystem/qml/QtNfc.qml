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
    packageName: "io.qt.nfc"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtNfc"
    module: "qtjambi.nfc"
    description: "Provides access to Near-Field communication (NFC) hardware."

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "#if defined(Q_OS_ANDROID)\nvoid initialize_meta_info_QtNfc();\n#endif"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "#if defined(Q_OS_ANDROID)\ninitialize_meta_info_QtNfc();\n#endif"}
    }
    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.ProvideOnly
    }
    
    Rejection{
        className: "RequestIdPrivate"
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
            Text{content: String.raw`
namespace QtJambiPrivate{
    template<>
    struct supports_less_than<QNdefMessage> : std::false_type{};
    template<>
    struct supports_stream_operators<QNdefMessage> : std::false_type{};
    template<>
    struct supports_debugstream<QNdefMessage> : std::false_type{};
}`}
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
            Text{content: String.raw`
@QtUninvokable
public final void appendRecord(Class<? extends QNdefRecord> type) {
    appendRecord(type, 1, 1);
}

@QtUninvokable
public final void appendRecord(Class<? extends QNdefRecord> type, int min) {
    appendRecord(type, min, 1);
}

@QtUninvokable
public final void appendRecord(Class<? extends QNdefRecord> type, int min, int max) {
    if(type==QNdefRecord.class) {
        QNdefRecord record = new QNdefRecord();
        appendRecord(record.typeNameFormat(), record.type(), min, max);
    }else if(type==QNdefNfcSmartPosterRecord.class) {
        QNdefNfcSmartPosterRecord record = new QNdefNfcSmartPosterRecord();
        appendRecord(record.typeNameFormat(), record.type(), min, max);
    }else if(type==QNdefNfcTextRecord.class) {
        QNdefNfcTextRecord record = new QNdefNfcTextRecord();
        appendRecord(record.typeNameFormat(), record.type(), min, max);
    }else if(type==QNdefNfcUriRecord.class) {
        QNdefNfcUriRecord record = new QNdefNfcUriRecord();
        appendRecord(record.typeNameFormat(), record.type(), min, max);
    }else if(type==QNdefNfcIconRecord.class) {
        QNdefNfcIconRecord record = new QNdefNfcIconRecord();
        appendRecord(record.typeNameFormat(), record.type(), min, max);
    }else {
        try {
            QNdefRecord record = type.getConstructor().newInstance();
            appendRecord(record.typeNameFormat(), record.type(), min, max);
        } catch (RuntimeException | Error e) {
            throw e;
        } catch (InstantiationException | IllegalAccessException
                                        | java.lang.reflect.InvocationTargetException | NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
    }
}`}
        }
    }
    
    ValueType{
        name: "QNdefMessage"
        noImplicitConstructors: true
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
namespace QtJambiPrivate{
    template<>
    struct supports_less_than<QNdefMessage> : std::false_type{};
    template<>
    struct supports_stream_operators<QNdefMessage> : std::false_type{};
    template<>
    struct supports_debugstream<QNdefMessage> : std::false_type{};
}
QT_WARNING_DISABLE_CLANG("-Wdeprecated-copy")
QT_WARNING_DISABLE_GCC("-Wdeprecated-copy")`}
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
        ModifyFunction{
            signature: "setPayload(const QByteArray &)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
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
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNdefFilter::Record."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNdefFilter."}
}
