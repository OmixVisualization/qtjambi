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
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
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
                Text{content: "if(!%0 && !%3.isEmpty()){\n"+
                              "    Java::QtSerialBus::QCanBusException::throwNew(%env, %3 QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
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
                Text{content: "}QTJAMBI_CATCH(const JavaException& exn){\n"+
                              "    if(Java::QtSerialBus::QCanBusException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%3){\n"+
                              "            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.object());\n"+
                              "            *%3 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
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
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(!%2.isEmpty()){\n"+
                              "    Java::QtSerialBus::QCanBusException::throwNew(%env, %2 QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
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
                Text{content: "}QTJAMBI_CATCH(const JavaException& exn){\n"+
                              "    if(Java::QtSerialBus::QCanBusException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%2){\n"+
                              "            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.object());\n"+
                              "            *%2 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
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
        ModifyFunction{
            signature: "setResetControllerFunction(std::function<void()>)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Runnable"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "core.runnable.function"
                    }
                }
            }
            until: 5
        }

        FunctionalType{
            name: "CanBusStatusGetter"
            generate: false
            using: "std::function<QCanBusDevice::CanBusStatus()>"
        }
        ModifyFunction{
            signature: "setCanBusStatusGetter(std::function<QCanBusDevice::CanBusStatus()>)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Supplier<@NonNull CanBusStatus>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "core.supplier.function"
                        Replace{
                            from: "%TYPE"
                            to: "QCanBusDevice::CanBusStatus"
                        }
                    }
                }
            }
            until: 5
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
            Text{content: "if(copy){\n"+
                          "    return new(placement) QCanBusDeviceInfo(*copy);\n"+
                          "}else{\n"+
                          "    struct CanBusDevice : QCanBusDevice{\n"+
                          "        static QCanBusDeviceInfo createDeviceInfo(){\n"+
                          "        return QCanBusDevice::createDeviceInfo({});\n"+
                          "    }\n"+
                          "};\n"+
                          "return new(placement) QCanBusDeviceInfo(CanBusDevice::createDeviceInfo());\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "struct CanBusDevice : QCanBusDevice{\n"+
                          "    static QCanBusDeviceInfo createDeviceInfo(){\n"+
                          "        return QCanBusDevice::createDeviceInfo({}, {}, false, false);\n"+
                          "    }\n"+
                          "};\n"+
                          "new(placement) QCanBusDeviceInfo(CanBusDevice::createDeviceInfo());"}
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
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
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
                Text{content: "if(!%0 && !%2.isEmpty()){\n"+
                              "    Java::QtSerialBus::QCanBusException::throwNew(%env, %2 QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
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
                Text{content: "}QTJAMBI_CATCH(const JavaException& exn){\n"+
                              "    if(Java::QtSerialBus::QCanBusException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%2){\n"+
                              "            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.object());\n"+
                              "            *%2 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
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
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(!%1.isEmpty()){\n"+
                              "    Java::QtSerialBus::QCanBusException::throwNew(%env, %1 QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
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
                Text{content: "}QTJAMBI_CATCH(const JavaException& exn){\n"+
                              "    if(Java::QtSerialBus::QCanBusException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%1){\n"+
                              "            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.object());\n"+
                              "            *%1 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
            }
            since: [6, 2]
        }
    }
    
    InterfaceType{
        name: "QCanBusFactoryV2"
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
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
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
                Text{content: "if(!%0 && !%2.isEmpty()){\n"+
                              "    Java::QtSerialBus::QCanBusException::throwNew(%env, %2 QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
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
                Text{content: "}QTJAMBI_CATCH(const JavaException& exn){\n"+
                              "    if(Java::QtSerialBus::QCanBusException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%2){\n"+
                              "            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.object());\n"+
                              "            *%2 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
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
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(!%1.isEmpty()){\n"+
                              "    Java::QtSerialBus::QCanBusException::throwNew(%env, %1 QTJAMBI_STACKTRACEINFO );\n"+
                              "}"}
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
                Text{content: "}QTJAMBI_CATCH(const JavaException& exn){\n"+
                              "    if(Java::QtSerialBus::QCanBusException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%1){\n"+
                              "            jstring message = Java::QtSerialBus::QCanBusException::getMessage(%env, exn.object());\n"+
                              "            *%1 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
            }
        }
        until: 5
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
            Text{content: "void* create_QModbusPdu(void* placement, const void * copy);\n"+
                          "return create_QModbusPdu(placement, copy);"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "void* create_QModbusPdu(void* placement, const void * copy);\n"+
                          "create_QModbusPdu(placement, copy);"}
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
            Text{content: "@QtUninvokable\n"+
                          "public void encodeData(Number...data) {\n"+
                          "    io.qt.core.QByteArray byteArray = new io.qt.core.QByteArray(data());\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(byteArray, io.qt.core.QIODevice.OpenModeFlag.WriteOnly);\n"+
                          "    for (Number s : data) {\n"+
                          "        if(s instanceof Byte) {\n"+
                          "            stream.writeByte((Byte)s);\n"+
                          "        }else if(s instanceof Short){\n"+
                          "            stream.writeShort((Short)s);\n"+
                          "        }else {\n"+
                          "            throw new IllegalArgumentException(\"Only byte and short supported.\");\n"+
                          "        }\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "    setData(byteArray);\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public void encodeData(short...data) {\n"+
                          "    io.qt.core.QByteArray byteArray = new io.qt.core.QByteArray(data());\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(byteArray, io.qt.core.QIODevice.OpenModeFlag.WriteOnly);\n"+
                          "    for (short s : data) {\n"+
                          "        stream.writeShort(s);\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "    setData(byteArray);\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public void encodeData(byte...data) {\n"+
                          "    io.qt.core.QByteArray byteArray = new io.qt.core.QByteArray(data());\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(byteArray, io.qt.core.QIODevice.OpenModeFlag.WriteOnly);\n"+
                          "    for (byte s : data) {\n"+
                          "        stream.writeByte(s);\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "    setData(byteArray);\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public void decodeData(byte[] data) {\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);\n"+
                          "    for (int i = 0; i < data.length; ++i) {\n"+
                          "        data[i] = stream.readByte();\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public void decodeData(short[] data) {\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);\n"+
                          "    for (int i = 0; i < data.length; ++i) {\n"+
                          "        data[i] = stream.readShort();\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "public void decodeData(Number[][] data) {\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);\n"+
                          "    for (int i = 0; i < data.length; ++i) {\n"+
                          "        if(data[i] instanceof Byte[]) {\n"+
                          "            ((Byte[])data[i])[0] = stream.readByte();\n"+
                          "        }else if(data[i] instanceof Short[]) {\n"+
                          "            ((Short[])data[i])[0] = stream.readShort();\n"+
                          "        }else {\n"+
                          "            stream.dispose();\n"+
                          "            throw new IllegalArgumentException(\"Only byte and short supported.\");\n"+
                          "        }\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "}"}
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
        name: "QModbusRtuSerialMaster"
        until: 5
    }
    
    ObjectType{
        name: "QModbusRtuSerialSlave"
        until: 5
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
