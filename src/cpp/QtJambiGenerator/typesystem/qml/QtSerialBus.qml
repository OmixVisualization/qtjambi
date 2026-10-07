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
    packageName: "io.qt.serialbus"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtSerialBus"
    module: "qtjambi.serialbus"
    description: "Provides access to serial industrial bus interface. Currently the module supports the CAN bus and Modbus protocols."

    RequiredLibrary{
        name: "QtNetwork"
    }
    
    RequiredLibrary{
        name: "QtSerialPort"
    }

    NamespaceType{
        name: "QtCanBus"

        EnumType{
            name: "UniqueId"
        }
        EnumType{
            name: "DataSource"
        }
        EnumType{
            name: "DataFormat"
        }
        EnumType{
            name: "DataEndian"
        }
        EnumType{
            name: "MultiplexState"
        }
        since: [6,5]
    }
    
    ObjectType{
        name: "QModbusClient"
        ModifyFunction{
            signature: "processPrivateResponse(const QModbusResponse &, QModbusDataUnit *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "processResponse(const QModbusResponse &, QModbusDataUnit *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QCanBus"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "createDevice(const QString &, const QString &, QString *) const"
            throwing: "QCanBusException"
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
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;`}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: String.raw`
                    if(!%0 && !%3.isEmpty()){
                        JavaException::raise<Java::QtSerialBus::QCanBusException>(%env, %3 QTJAMBI_STACKTRACEINFO );
                    }`}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "QTJAMBI_TRY{"}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: String.raw`
}QTJAMBI_CATCH(const JavaException& exn){
    if(exn.isInstanceOf(%env, Java::QtSerialBus::QCanBusException::getClass(%env))){
        if(%3){
            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.throwable(%env));
            *%3 = qtjambi_cast<QString>(%env, message);
        }
    }else{
        exn.raise();
    }
}QTJAMBI_TRY_END`}
            }
        }
        ModifyFunction{
            signature: "availableDevices(const QString &, QString *) const"
            throwing: "QCanBusException"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;`}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: String.raw`
                    if(!%2.isEmpty()){
                        JavaException::raise<Java::QtSerialBus::QCanBusException>(%env, %2 QTJAMBI_STACKTRACEINFO );
                    }`}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "QTJAMBI_TRY{"}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: String.raw`
}QTJAMBI_CATCH(const JavaException& exn){
    if(exn.isInstanceOf(%env, Java::QtSerialBus::QCanBusException::getClass(%env))){
        if(%2){
            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.throwable(%env));
            *%2 = qtjambi_cast<QString>(%env, message);
        }
    }else{
        exn.raise();
    }
}QTJAMBI_TRY_END`}
            }
        }
    }
    
    ObjectType{
        name: "QCanBusDevice"

        ValueType{
            name: "Filter"

            EnumType{
                name: "FormatFilter"
            }
        }

        EnumType{
            name: "CanBusDeviceState"
        }

        EnumType{
            name: "CanBusError"
        }

        EnumType{
            name: "CanBusStatus"
        }

        EnumType{
            name: "ConfigurationKey"
        }

        EnumType{
            name: "Direction"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "clear(QCanBusDevice::Directions)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "io.qt.serialbus.QCanBusDevice.Direction.AllDirections"
                }
            }
        }

        FunctionalType{
            name: "ResetControllerFunction"
            generate: false
            using: "std::function<void()>"
        }
        FunctionalType{
            name: "CanBusStatusGetter"
            generate: false
            using: "std::function<QCanBusDevice::CanBusStatus()>"
        }
    }
    
    ValueType{
        name: "QCanBusDeviceInfo"
        ExtraIncludes{
            Include{
                fileName: "QCanBusDevice"
                location: Include.Global
            }
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: String.raw`
                struct CanBusDevice : QCanBusDevice{
                    static QCanBusDeviceInfo createDeviceInfo(){
                        return QCanBusDevice::createDeviceInfo({}, {}, false, false);
                    }
                };
                new(placement) QCanBusDeviceInfo(CanBusDevice::createDeviceInfo());`}
        }
    }
    
    InterfaceType{
        name: "QCanBusFactory"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "createDevice(const QString &, QString *) const"
            throwing: "QCanBusException"
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
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;`}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: String.raw`
                    if(!%0 && !%2.isEmpty()){
                        JavaException::raise<Java::QtSerialBus::QCanBusException>(%env, %2 QTJAMBI_STACKTRACEINFO );
                    }`}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "QTJAMBI_TRY{"}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: String.raw`
                    }QTJAMBI_CATCH(const JavaException& exn){
                        if(exn.isInstanceOf(%env, Java::QtSerialBus::QCanBusException::getClass(%env))){
                            if(%2){
                                jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.throwable(%env));
                                *%2 = qtjambi_cast<QString>(%env, message);
                            }
                        }else{
                            exn.raise();
                        }
                    }QTJAMBI_TRY_END`}
            }
        }
        ModifyFunction{
            signature: "availableDevices(QString *) const"
            throwing: "QCanBusException"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;`}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: String.raw`
                    if(!%1.isEmpty()){
                        JavaException::raise<Java::QtSerialBus::QCanBusException>(%env, %1 QTJAMBI_STACKTRACEINFO );
                    }`}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.Beginning
                Text{content: "QTJAMBI_TRY{"}
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: String.raw`
                    }QTJAMBI_CATCH(const JavaException& exn){
                        if(exn.isInstanceOf(%env, Java::QtSerialBus::QCanBusException::getClass(%env))){
                            if(%1){
                                jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.throwable(%env));
                                *%1 = qtjambi_cast<QString>(%env, message);
                            }
                        }else{
                            exn.raise();
                        }
                    }QTJAMBI_TRY_END`}
            }
            since: [6, 2]
        }
    }
    
    ValueType{
        name: "QCanBusFrame"

        EnumType{
            name: "FrameError"
        }

        EnumType{
            name: "FrameType"
        }

        ValueType{
            name: "TimeStamp"
            noImplicitConstructors: true
        }
    }
    
    ValueType{
        name: "QModbusDataUnit"

        EnumType{
            name: "RegisterType"
        }
    }
    
    ObjectType{
        name: "QModbusDevice"

        EnumType{
            name: "IntermediateError"
            since: [6, 2]
        }

        EnumType{
            name: "ConnectionParameter"
        }

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "State"
        }
    }
    
    EnumType{
        name: "QModbusDeviceIdentification::ConformityLevel"
    }
    
    EnumType{
        name: "QModbusDeviceIdentification::ObjectId"
    }
    
    EnumType{
        name: "QModbusDeviceIdentification::ReadDeviceIdCode"
    }
    
    ValueType{
        name: "QModbusDeviceIdentification"
    }
    
    ValueType{
        name: "QModbusPdu"

        Rejection{
            className: "IsType"
        }

        EnumType{
            name: "ExceptionCode"
        }

        EnumType{
            name: "FunctionCode"
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: String.raw`
                void* create_QModbusPdu(void* placement, const void * copy);
                create_QModbusPdu(placement, copy);`}
        }
        ModifyFunction{
            signature: "operator<<(QDataStream &,QModbusPdu)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "encodeData<Args...>(Args)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "decodeData<Args...>(Args&&)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QModbusPdu<Args...>(QModbusPdu::FunctionCode,Args)"
            remove: RemoveFlag.All
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiSerialBus.java"
                quoteAfterLine: "class QModbusPdu__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QModbusRequest"

        FunctionalType{
            name: "CalcFunction"
            using: "int(*)(const QModbusRequest &)"
        }

        FunctionalType{
            name: "CalcFuncPtr"
            javaName: "CalcFunction"
            generate: false
        }
        ModifyFunction{
            signature: "QModbusRequest<Args...>(QModbusPdu::FunctionCode,Args)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "registerDataSizeCalculator(QModbusPdu::FunctionCode,QModbusRequest::CalcFuncPtr)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcDataSizeCalculators"
                    keyArgument: 1
                    action: ReferenceCount.Put
                }
            }
        }
    }
    
    ValueType{
        name: "QModbusResponse"

        FunctionalType{
            name: "CalcFunction"
            using: "int(*)(const QModbusResponse &)"
        }

        FunctionalType{
            name: "CalcFuncPtr"
            javaName: "CalcFunction"
            generate: false
        }
        ModifyFunction{
            signature: "QModbusResponse<Args...>(QModbusPdu::FunctionCode,Args)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "registerDataSizeCalculator(QModbusPdu::FunctionCode,QModbusResponse::CalcFuncPtr)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcDataSizeCalculators"
                    keyArgument: 1
                    action: ReferenceCount.Put
                }
            }
        }
    }
    
    ValueType{
        name: "QModbusExceptionResponse"
    }
    
    ObjectType{
        name: "QModbusReply"

        EnumType{
            name: "ReplyType"
        }
    }
    
    ObjectType{
        name: "QModbusRtuSerialServer"
        ppCondition: "!defined(Q_OS_IOS)"
        since: [6, 2]
    }

    ObjectType{
        name: "QModbusRtuSerialClient"
        ppCondition: "!defined(Q_OS_IOS)"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QModbusServer"

        EnumType{
            name: "Option"
        }
        ModifyFunction{
            signature: "data(QModbusDataUnit *) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "readData(QModbusDataUnit *) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "data(QModbusDataUnit::RegisterType,quint16,quint16*)const"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 0
                }
            }
        }
    }
    
    ObjectType{
        name: "QModbusTcpClient"
        ExtraIncludes{
            Include{
                fileName: "qtcpsocket.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QModbusTcpServer"
        ExtraIncludes{
            Include{
                fileName: "qtcpsocket.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "installConnectionObserver(QModbusTcpConnectionObserver *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "modbusClientDisconnected(QTcpSocket *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.@Nullable QTcpSocket"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QTcpSocket* %out = QtJambiAPI::convertJavaObjectToNative<QTcpSocket>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, reinterpret_cast<void*>(%in), Java::QtNetwork::QTcpSocket::getClass(%env));"}
                }
            }
        }
    }
    
    InterfaceType{
        name: "QModbusTcpConnectionObserver"
        ExtraIncludes{
            Include{
                fileName: "qtcpsocket.h"
                suppressed: true
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "acceptNewConnection(QTcpSocket *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.@Nullable QTcpSocket"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QTcpSocket* %out = QtJambiAPI::convertJavaObjectToNative<QTcpSocket>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, reinterpret_cast<void*>(%in), Java::QtNetwork::QTcpSocket::getClass(%env));"}
                }
            }
        }
    }

    ObjectType{
        name: "QCanDbcFileParser"

        EnumType{
            name: "Error"
        }
        since: [6,5]
    }

    ObjectType{
        name: "QCanFrameProcessor"

        EnumType{
            name: "Error"
        }

        ValueType{
            name: "ParseResult"
        }
        since: [6,5]
    }

    ObjectType{
        name: "QCanMessageDescription"
        since: [6,5]
    }

    ObjectType{
        name: "QCanSignalDescription"
        ValueType{
            name: "MultiplexValueRange"
        }
        since: [6,5]
    }

    ObjectType{
        name: "QCanUniqueIdDescription"
        since: [6,5]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QModbusPdu::encode*', unmatched parameter type 'const QList*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QModbusPdu::encode', unmatched parameter type '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Class 'QCanSignalDescription::MultiplexValueRange' has equals operators but no qHash() function. Hashcode of objects will consistently be 0."}
}
