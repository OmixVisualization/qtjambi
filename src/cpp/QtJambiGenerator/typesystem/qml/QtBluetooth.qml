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
    packageName: "io.qt.bluetooth"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtBluetooth"
    module: "qtjambi.bluetooth"
    description: "Provides access to Bluetooth hardware."

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QtBluetooth();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_QtBluetooth();"}
    }

    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.Mandatory
        platforms: ["linux"]
        since: 6
    }
    
    RequiredLibrary{
        name: "QtConcurrent"
        mode: RequiredLibrary.Mandatory
        until: 5
    }

    RequiredLibrary{
        name: "QtNetwork"
        mode: RequiredLibrary.Mandatory
        until: 5
    }
    
    Rejection{
        className: "QBluetoothSocketBasePrivate"
    }
    
    NamespaceType{
        name: "QBluetooth"
        hasMetaObject: true

        EnumType{
            name: "AttAccessConstraint"
        }

        EnumType{
            name: "Security"
        }
    }
    
    ValueType{
        name: "QBluetoothAddress"
    }
    
    ValueType{
        name: "QBluetoothServiceInfo"

        EnumType{
            name: "AttributeId"
            RejectEnumValue{
                name: "PrimaryLanguageBase"
            }
        }

        EnumType{
            name: "Protocol"
        }

        ValueType{
            name: "Sequence"
            ExtraIncludes{
                Include{
                    fileName: "hashes.h"
                    location: Include.Local
                }
            }
        }

        ValueType{
            name: "Alternative"
            ExtraIncludes{
                Include{
                    fileName: "hashes.h"
                    location: Include.Local
                }
            }
        }
    }
    
    ValueType{
        name: "QBluetoothDeviceInfo"

        EnumType{
            name: "CoreConfiguration"
        }

        EnumType{
            name: "DataCompleteness"
        }

        EnumType{
            name: "Field"
        }

        EnumType{
            name: "MajorDeviceClass"
            RejectEnumValue{
                name: "NetworkDevice"
                until: 5
            }
        }

        EnumType{
            name: "MinorAudioVideoClass"
        }

        EnumType{
            name: "MinorComputerClass"
        }

        EnumType{
            name: "MinorHealthClass"
        }

        EnumType{
            name: "MinorImagingClass"
        }

        EnumType{
            name: "MinorMiscellaneousClass"
        }

        EnumType{
            name: "MinorNetworkClass"
        }

        EnumType{
            name: "MinorPeripheralClass"
        }

        EnumType{
            name: "MinorPhoneClass"
        }

        EnumType{
            name: "MinorToyClass"
        }

        EnumType{
            name: "MinorWearableClass"
        }

        EnumType{
            name: "ServiceClass"
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            until: [5, 13]
            Text{content: "public final static class ServiceUuids implements Iterable<QBluetoothUuid>{\n"+
                          "    private ServiceUuids(java.util.List<QBluetoothUuid> serviceUuids, DataCompleteness completeness) {\n"+
                          "        super();\n"+
                          "        this.serviceUuids = serviceUuids;\n"+
                          "        this.completeness = completeness;\n"+
                          "    }\n"+
                          "\n"+
                          "    public final java.util.List<io.qt.bluetooth.QBluetoothUuid> serviceUuids;\n"+
                          "    public final DataCompleteness completeness;\n"+
                          "\n"+
                          "    @Override\n"+
                          "    public java.util.Iterator<QBluetoothUuid> iterator() {\n"+
                          "        return serviceUuids.iterator();\n"+
                          "    }\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "serviceUuids(QBluetoothDeviceInfo::DataCompleteness *) const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QBluetoothDeviceInfo::DataCompleteness* %out = nullptr;"}
                }
            }
            since: [5, 14]
            until: 5
        }
        ModifyFunction{
            signature: "serviceUuids(QBluetoothDeviceInfo::DataCompleteness *) const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QBluetoothDeviceInfo::DataCompleteness completeness;\n"+
                                  "QBluetoothDeviceInfo::DataCompleteness* %out = &completeness;"}
                }
            }
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.bluetooth.QBluetoothDeviceInfo$ServiceUuids"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtBluetooth::QBluetoothDeviceInfo$ServiceUuids::newInstance(%env, qtjambi_cast<jobject>(%env, %in), qtjambi_cast<jobject>(%env, completeness));"}
                }
            }
            until: [5, 13]
        }
    }
    
    ValueType{
        name: "QBluetoothHostInfo"
    }
    
    ValueType{
        name: "QBluetoothTransferRequest"

        EnumType{
            name: "Attribute"
        }
        until: 5
    }
    
    ValueType{
        name: "QBluetoothUuid"
        Rejection{functionName: "fromCBUUID"}
        Rejection{functionName: "toCBUUID"}
        EnumType{
            name: "CharacteristicType"
        }

        EnumType{
            name: "DescriptorType"
        }

        EnumType{
            name: "ProtocolUuid"
        }

        EnumType{
            name: "ServiceClassUuid"
        }
        ModifyFunction{
            signature: "toUInt128() const"
            rename: "toBytes"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "byte[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Int8PointerArray %inArray(%env, reinterpret_cast<qint8*>(%in.data), 16);\n"+
                                  "%out = %env->NewByteArray(16);\n"+
                                  "%env->SetByteArrayRegion(jbyteArray(%out), 0, 16, *reinterpret_cast<jbyte **>(&%in));"}
                }
            }
            until: 6.5
        }
        ModifyFunction{
            signature: "QBluetoothUuid(quint128)"
            InjectCode{
                Text{content: "if(uuid.length!=16)\n"+
                              "    throw new IllegalArgumentException(\"Uuid needs to be an array of length 16.\");"}
            }
            ModifyArgument{
                index: 1
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "byte[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "quint128 %out;\n"+
                                  "%env->GetByteArrayRegion(jbyteArray(%in), 0, 16, *reinterpret_cast<jbyte **>(&%out));"}
                }
            }
            until: 6.5
        }
        ModifyFunction{
            signature: "toUInt128(QSysInfo::Endian) const"
            remove: RemoveFlag.All
            since: 6.6
        }
        ModifyFunction{
            signature: "operator==(QBluetoothUuid)const"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "QUuid"}
            }
            until: 5
        }
        ModifyFunction{
            signature: "operator==(QBluetoothUuid,QBluetoothUuid)"
            ModifyArgument{
                index: 2
                InhibitImplicitCall{type: "QUuid"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "QBluetoothUuid<>(quint128, QSysInfo::Endian)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.core.QUuid$Id128Bytes"
                }
                AddImplicitCall{type: "byte @NonNull[]"}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QUuid::Id128Bytes %out = qtjambi_cast<QUuid::Id128Bytes>(%env, %in);"}
                }
            }
            since: 6.6
        }
        ModifyFunction{
            signature: "toUInt16(bool*) const"
            rename: "toShort"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Short"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaShortObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
        ModifyFunction{
            signature: "toUInt32(bool*) const"
            rename: "toInt"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Integer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaIntegerObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "quint128"
        generate: false
    }
    
    ValueType{
        name: "QLowEnergyAdvertisingData"

        EnumType{
            name: "Discoverability"
        }
    }
    
    ValueType{
        name: "QLowEnergyAdvertisingParameters"

        EnumType{
            name: "FilterPolicy"
        }

        EnumType{
            name: "Mode"
        }

        ValueType{
            name: "AddressInfo"
        }
    }
    
    ValueType{
        name: "QLowEnergyDescriptor"
    }
    
    ValueType{
        name: "QLowEnergyDescriptorData"
    }
    
    ValueType{
        name: "QLowEnergyCharacteristic"

        EnumType{
            name: "PropertyType"
        }
    }
    
    ValueType{
        name: "QLowEnergyCharacteristicData"
    }
    
    ValueType{
        name: "QLowEnergyConnectionParameters"
    }
    
    ValueType{
        name: "QLowEnergyServiceData"

        EnumType{
            name: "ServiceType"
        }
        ModifyFunction{
            signature: "setIncludedServices(const QList<QLowEnergyService*>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcIncludedService!=null){\n"+
                              "    __rcIncludedService.clear();\n"+
                              "}else{\n"+
                              "    __rcIncludedService = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcIncludedService.addAll(services);"}
            }
        }
        ModifyFunction{
            signature: "addIncludedService(QLowEnergyService *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIncludedService"
                    action: ReferenceCount.Add
                }
            }
        }
    }
    
    ObjectType{
        name: "QBluetoothDeviceDiscoveryAgent"

        EnumType{
            name: "DiscoveryMethod"
        }

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "InquiryType"
        }
    }
    
    ObjectType{
        name: "QBluetoothLocalDevice"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "HostMode"
        }

        EnumType{
            name: "Pairing"
        }
    }
    
    ObjectType{
        name: "QBluetoothSocket"

        EnumType{
            name: "SocketError"
        }

        EnumType{
            name: "SocketState"
        }
        ModifyFunction{
            signature: "errorString() const"
            rename: "socketErrorString"
        }
        ModifyFunction{
            signature: "connectToService(QBluetoothAddress,QBluetoothUuid,QIODevice::OpenMode)"
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
            until: 5
        }
        ModifyFunction{
            signature: "connectToService(QBluetoothAddress,QBluetoothUuid,QIODeviceBase::OpenMode)"
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QBluetoothServer"

        EnumType{
            name: "Error"
        }
    }
    
    ObjectType{
        name: "QBluetoothServiceDiscoveryAgent"

        EnumType{
            name: "DiscoveryMode"
        }

        EnumType{
            name: "Error"
        }
    }
    
    ObjectType{
        name: "QBluetoothTransferManager"
    }
    
    ObjectType{
        name: "QBluetoothTransferReply"

        EnumType{
            name: "TransferError"
            until: 5
        }
        ModifyFunction{
            signature: "setManager(QBluetoothTransferManager*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcManager"
                    action: ReferenceCount.Set
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QLowEnergyController"

        EnumType{
            name: "ControllerState"
        }

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "RemoteAddressType"
        }

        EnumType{
            name: "Role"
        }
        ModifyFunction{
            signature: "addService(QLowEnergyServiceData,QObject*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "createPeripheral(QBluetoothAddress,QObject*)"
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
            since: [6, 2]
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "createServiceObject(const QBluetoothUuid &, QObject *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createCentral(const QBluetoothDeviceInfo &, QObject *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createCentral(const QBluetoothDeviceInfo &,QBluetoothAddress,QObject *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "createCentral(QBluetoothAddress,QBluetoothAddress,QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "createPeripheral(QObject *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "QLowEnergyService"

        EnumType{
            name: "WriteMode"
        }

        EnumType{
            name: "DiscoveryMode"
            since: [6, 2]
        }

        EnumType{
            name: "ServiceType"
        }

        EnumType{
            name: "ServiceError"
        }

        EnumType{
            name: "ServiceState"
            RejectEnumValue{
                name: "DiscoveryRequired"
                since: [6, 2]
            }
            RejectEnumValue{
                name: "DiscoveringService"
            }
            RejectEnumValue{
                name: "ServiceDiscovered"
                since: [6, 2]
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Object type 'quint128' passed as value. Resulting code will not compile.  io.qt.bluetooth.QBluetoothUuid::QBluetoothUuid(quint128 uuid)"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcIncludedService' in 'QLowEnergyServiceData' but not both."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched parameter type 'QSharedPointer<*Private>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping field '*' with unmatched type '*Private'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QBluetooth::operator|', unmatched return type '*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QBluetoothServiceInfo::*."}
}
