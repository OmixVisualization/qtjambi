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
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtSerialBus"
    module: "qtjambi.serialbus"
    description: "Provides access to serial industrial bus interface. Currently the module supports the CAN bus and Modbus protocols."

    RequiredLibrary{
        name: "QtNetwork"
    }
    
    RequiredLibrary{
        name: "QtSerialPort"
    }
    
    Rejection{
        className: "QModbusPdu::IsType"
    }

    NamespaceType{
        name: "QtCanBus"
        since: [6,5]
    }

    EnumType{
        name: "QtCanBus::DataSource"
        since: [6,5]
    }

    EnumType{
        name: "QtCanBus::DataFormat"
        since: [6,5]
    }

    EnumType{
        name: "QtCanBus::DataEndian"
        since: [6,5]
    }

    EnumType{
        name: "QtCanBus::MultiplexState"
        since: [6,5]
    }
    
    ObjectType{
        name: "QModbusClient"
        ModifyFunction{
            signature: "processPrivateResponse(const QModbusResponse &, QModbusDataUnit *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.serialbus.QModbusDataUnit"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QModbusDataUnit* %out = qtjambi_cast<QModbusDataUnit*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "processResponse(const QModbusResponse &, QModbusDataUnit *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.serialbus.QModbusDataUnit"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QModbusDataUnit* %out = qtjambi_cast<QModbusDataUnit*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
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
    
    EnumType{
        name: "QCanBusDevice::CanBusDeviceState"
    }
    
    EnumType{
        name: "QCanBusDevice::CanBusError"
    }
    
    EnumType{
        name: "QCanBusDevice::CanBusStatus"
    }
    
    EnumType{
        name: "QCanBusDevice::ConfigurationKey"
    }
    
    EnumType{
        name: "QCanBusDevice::Direction"
        flags: "QCanBusDevice::Directions"
    }
    
    ObjectType{
        name: "QCanBusDevice"
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
            signature: "clear(QFlags<QCanBusDevice::Direction>)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "io.qt.serialbus.QCanBusDevice.Direction.AllDirections"
                }
            }
        }
        ModifyFunction{
            signature: "setResetControllerFunction(std::function<void()>)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Runnable"
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
        ModifyFunction{
            signature: "setCanBusStatusGetter(std::function<QCanBusDevice::CanBusStatus()>)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.Supplier<CanBusStatus>"
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
    
    EnumType{
        name: "QCanBusDevice::Filter::FormatFilter"
        flags: "QCanBusDevice::Filter::FormatFilters"
    }
    
    ValueType{
        name: "QCanBusDevice::Filter"
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
        ModifyFunction{
            signature: "operator=(const QCanBusDeviceInfo &)"
            remove: RemoveFlag.All
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
    
    EnumType{
        name: "QCanBusFrame::FrameError"
        flags: "QCanBusFrame::FrameErrors"
    }
    
    EnumType{
        name: "QCanBusFrame::FrameType"
    }
    
    ValueType{
        name: "QCanBusFrame"
    }
    
    ValueType{
        name: "QCanBusFrame::TimeStamp"
    }
    
    EnumType{
        name: "QModbusDataUnit::RegisterType"
    }
    
    ValueType{
        name: "QModbusDataUnit"
    }
    
    EnumType{
        name: "QModbusDevice::IntermediateError"
        since: [6, 2]
    }
    
    EnumType{
        name: "QModbusDevice::ConnectionParameter"
    }
    
    EnumType{
        name: "QModbusDevice::Error"
    }
    
    EnumType{
        name: "QModbusDevice::State"
    }
    
    ObjectType{
        name: "QModbusDevice"
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
    
    EnumType{
        name: "QModbusPdu::ExceptionCode"
    }
    
    EnumType{
        name: "QModbusPdu::FunctionCode"
    }
    
    ValueType{
        name: "QModbusPdu"
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
            signature: "operator=(const QModbusPdu &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(QDataStream &,QModbusPdu)"
            access: Modification.NonFinal
        }
        InjectCode{
            Text{content: "@io.qt.QtUninvokable\n"+
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
                          "@io.qt.QtUninvokable\n"+
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
                          "@io.qt.QtUninvokable\n"+
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
                          "@io.qt.QtUninvokable\n"+
                          "public void decodeData(byte[] data) {\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);\n"+
                          "    for (int i = 0; i < data.length; ++i) {\n"+
                          "        data[i] = stream.readByte();\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "}\n"+
                          "\n"+
                          "@io.qt.QtUninvokable\n"+
                          "public void decodeData(short[] data) {\n"+
                          "    io.qt.core.QDataStream stream = new io.qt.core.QDataStream(data(), io.qt.core.QIODevice.OpenModeFlag.ReadOnly);\n"+
                          "    for (int i = 0; i < data.length; ++i) {\n"+
                          "        data[i] = stream.readShort();\n"+
                          "    }\n"+
                          "    stream.dispose();\n"+
                          "}\n"+
                          "\n"+
                          "@io.qt.QtUninvokable\n"+
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
    
    FunctionalType{
        name: "QModbusRequestCalculatorFunction"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    FunctionalType{
        name: "QModbusResponseCalculatorFunction"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    FunctionalType{
        name: "QModbusRequest::CalcFuncPtr"
        generate: false
    }
    
    FunctionalType{
        name: "QModbusResponse::CalcFuncPtr"
        generate: false
    }
    
    ValueType{
        name: "QModbusRequest"
        ModifyFunction{
            signature: "registerDataSizeCalculator(QModbusPdu::FunctionCode, QModbusRequest::CalcFuncPtr)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "io.qt.serialbus.QModbusRequestCalculatorFunction"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QModbusRequest::CalcFuncPtr %out = qtjambi_cast<QModbusRequest::CalcFuncPtr>(%env, %in);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QModbusResponse"
        ModifyFunction{
            signature: "registerDataSizeCalculator(QModbusPdu::FunctionCode, QModbusResponse::CalcFuncPtr)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "io.qt.serialbus.QModbusResponseCalculatorFunction"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QModbusResponse::CalcFuncPtr %out = qtjambi_cast<QModbusResponse::CalcFuncPtr>(%env, %in);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QModbusExceptionResponse"
    }
    
    EnumType{
        name: "QModbusReply::ReplyType"
    }
    
    ObjectType{
        name: "QModbusReply"
    }
    
    ObjectType{
        name: "QModbusRtuSerialMaster"
        until: 5
    }
    
    ObjectType{
        name: "QModbusRtuSerialClient"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QModbusRtuSerialSlave"
        until: 5
    }
    
    ObjectType{
        name: "QModbusRtuSerialServer"
        since: [6, 2]
    }
    
    EnumType{
        name: "QModbusServer::Option"
    }
    
    ObjectType{
        name: "QModbusServer"
        ModifyFunction{
            signature: "data(QModbusDataUnit *) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.serialbus.QModbusDataUnit"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QModbusDataUnit* %out = qtjambi_cast<QModbusDataUnit*>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "readData(QModbusDataUnit *) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReplaceType{
                    modifiedType: "io.qt.serialbus.QModbusDataUnit"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QModbusDataUnit* %out = qtjambi_cast<QModbusDataUnit*>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "data(QModbusDataUnit::RegisterType,unsigned short,unsigned short*)const"
            ModifyArgument{
                index: 3
                ArrayType{
                    minLength: 1
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
                    modifiedType: "io.qt.network.QTcpSocket"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QTcpSocket* %out = QtJambiAPI::convertJavaObjectToNative<QTcpSocket>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = QtJambiAPI::convertNativeToJavaObject(%env, reinterpret_cast<void*>(%in), \"io/qt/network/QTcpSocket\", false, false);"}
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
                    modifiedType: "io.qt.network.QTcpSocket"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QTcpSocket* %out = QtJambiAPI::convertJavaObjectToNative<QTcpSocket>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObject(%env, reinterpret_cast<void*>(%in), \"io/qt/network/QTcpSocket\", false, false);"}
                }
            }
        }
    }
    
    FunctionalType{
        name: "QCanBusDevice::ResetControllerFunction"
        generate: false
        using: "std::function<void()>"
    }
    
    FunctionalType{
        name: "QCanBusDevice::CanBusStatusGetter"
        generate: false
        using: "std::function<QCanBusDevice::CanBusStatus()>"
    }

    ObjectType{
        name: "QCanDbcFileParser"
        since: [6,5]
    }

    EnumType{
        name: "QCanDbcFileParser::Error"
        since: [6,5]
    }

    ValueType{
        name: "QCanFrameProcessor"
        ModifyFunction{
            signature: "operator=(const QCanFrameProcessor &)"
            remove: RemoveFlag.All
        }
        since: [6,5]
    }

    EnumType{
        name: "QCanFrameProcessor::Error"
        since: [6,5]
    }

    ValueType{
        name: "QCanFrameProcessor::ParseResult"
        since: [6,5]
    }

    ValueType{
        name: "QCanMessageDescription"
        ModifyFunction{
            signature: "operator=(const QCanMessageDescription &)"
            remove: RemoveFlag.All
        }
        since: [6,5]
    }

    ValueType{
        name: "QCanSignalDescription"
        ModifyFunction{
            signature: "operator=(const QCanSignalDescription &)"
            remove: RemoveFlag.All
        }
        since: [6,5]
    }

    ValueType{
        name: "QCanUniqueIdDescription"
        ModifyFunction{
            signature: "operator=(const QCanUniqueIdDescription &)"
            remove: RemoveFlag.All
        }
        since: [6,5]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: function '*::CalcFuncPtr' is specified in typesystem, but not declared"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QModbusDataUnit."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QModbusDeviceIdentification."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QModbusPdu."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QCanBus*."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QModbusPdu::encode*', unmatched parameter type 'const QList*'"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QCanBusDeviceInfo' is missing a default constructor. The resulting C++ code will not compile.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Class function '*::CalcFuncPtr' is specified in typesystem, but not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QModbusPdu::encode', unmatched parameter type '*'"}
}
