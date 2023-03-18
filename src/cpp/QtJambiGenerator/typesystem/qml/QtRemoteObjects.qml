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
    packageName: "io.qt.remoteobjects"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtRemoteObjects"
    module: "qtjambi.remoteobjects"
    description: "Provides an easy to use mechanism for sharing a QObject's API (Properties/Signals/Slots) between processes or devices."

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_RemoteObject();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_RemoteObject();"}
    }

    Rejection{
        className: "*"
        functionName: "getTypeNameAndMetaobjectFromClassInfo"
    }
    
    Rejection{
        className: "ExternalIoDevice"
    }
    
    Rejection{
        className: "QtROServerFactory"
    }
    
    Rejection{
        className: "QtROClientFactory"
    }
    
    Rejection{
        className: "QRemoteObjectPendingCallData"
    }
    
    Rejection{
        className: "QtRemoteObjects"
        fieldName: "dataStreamVersion"
    }
    
    Rejection{
        className: "QtRemoteObjects"
        fieldName: "protocolVersion"
    }
    
    Rejection{
        className: "QtRemoteObjects"
        functionName: "to_underlying"
    }
    
    NamespaceType{
        name: "QRemoteObjectStringLiterals"
    }
    
    NamespaceType{
        name: "QtRemoteObjects"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "operator<<(QDataStream &, QItemSelectionModel::SelectionFlags)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(QDataStream &, Qt::Orientation)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(QDataStream &, QItemSelectionModel::SelectionFlags &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator>>(QDataStream &, Qt::Orientation &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyStoredProperties(const QMetaObject *, QDataStream &, void *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyStoredProperties(const QMetaObject *, const void *, QDataStream &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyStoredProperties(const QMetaObject *, const void *, void *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyStoredProperties<T>(const T *, T *)"
            Instantiation{
                proxyCall: "qtjambi_copyStoredProperties"
                Argument{
                    type: "_jobject"
                }
                AddTypeParameter{
                    name: "T"
                }
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                }
                AddArgument{
                    index: 1
                    name: "type"
                    type: "java.lang.Class<T>"
                }
            }
        }
        ModifyFunction{
            signature: "copyStoredProperties<T>(const T *, QDataStream &)"
            Instantiation{
                proxyCall: "qtjambi_copyStoredProperties"
                Argument{
                    type: "_jobject"
                }
                AddTypeParameter{
                    name: "T"
                }
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                }
                AddArgument{
                    index: 1
                    name: "type"
                    type: "java.lang.Class<T>"
                }
            }
        }
        ModifyFunction{
            signature: "copyStoredProperties<T>(QDataStream &,T *)"
            Instantiation{
                proxyCall: "qtjambi_copyStoredProperties"
                Argument{
                    type: "_jobject"
                }
                AddTypeParameter{
                    name: "T"
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                }
                AddArgument{
                    index: 1
                    name: "type"
                    type: "java.lang.Class<T>"
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class QtRemoteObjects___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    EnumType{
        name: "QtRemoteObjects::QRemoteObjectPacketTypeEnum"
        hiddenMetaobject: true
    }
    
    EnumType{
        name: "QtRemoteObjects::InitialAction"
        hiddenMetaobject: true
    }
    
    EnumType{
        name: "QRemoteObjectHostBase::AllowedSchemas"
    }
    
    EnumType{
        name: "QRemoteObjectPendingCall::Error"
    }
    
    ObjectType{
        name: "QRemoteObjectAbstractPersistedStore"
    }
    
    ObjectType{
        name: "QRemoteObjectDynamicReplica"
    }
    
    ObjectType{
        name: "QRemoteObjectHost"
    }
    
    ObjectType{
        name: "QRemoteObjectHostBase"
        ModifyFunction{
            signature: "proxy(const QUrl &, const QUrl &, QRemoteObjectHostBase::RemoteObjectNameFilter)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.core.QUrl()"
                }
            }
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "(a,b)->true"
                }
            }
        }
        ModifyFunction{
            signature: "reverseProxy(QRemoteObjectHostBase::RemoteObjectNameFilter)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "(a,b)->true"
                }
            }
        }
        ModifyFunction{
            signature: "addHostSideConnection(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcHostSideConnection"
                    action: ReferenceCount.Add
                }
            }
        }
    }
    
    FunctionalType{
        name: "QRemoteObjectHostBase::RemoteObjectNameFilter"
    }
    
    ObjectType{
        name: "QRemoteObjectNode"
        EnumType{
            name: "ErrorCode"
        }
        ModifyFunction{
            signature: "setPersistedStore(QRemoteObjectAbstractPersistedStore*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPersistedStore"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "addClientSideConnection(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcHostSideConnection"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "acquire<ObjectType>(QString)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "instances<T>() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "acquireModel(const QString &, QtRemoteObjects::InitialAction, const QVector<int> &)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "new java.util.ArrayList<>()"
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class QRemoteObjectNode___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    FunctionalType{
        name: "QRemoteObjectNode::RemoteObjectSchemaHandler"
    }
    
    ObjectType{
        name: "QRemoteObjectRegistry"
    }
    
    ObjectType{
        name: "QRemoteObjectRegistryHost"
    }
    
    ObjectType{
        name: "QRemoteObjectReplica"
        Rejection{fieldName: "d_impl"}
        EnumType{
            name: "State"
        }

        EnumType{
            name: "ConstructorType"
        }
        ModifyFunction{
            signature: "setNode(QRemoteObjectNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNode"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractItemModelReplica"
    }
    
    ObjectType{
        name: "QRemoteObjectSettingsStore"
    }
    
    ObjectType{
        name: "QConnectionAbstractServer"
        until: 5
        unless: "QTJAMBI_NO_REMOTEOBJECTS_PRIVATE"
    }
    
    ObjectType{
        name: "QConnectionAbstractServer"
        since: 6
    }
    
    ObjectType{
        name: "IoDeviceBase"
        ModifyField{
            name: "m_isClosing"
            read: false
            write: true
            rename: "Closing"
        }
        InjectCode{
            target: CodeClass.DestructorFunction
            Text{content: "if (!%this->m_isClosing)\n"+
                          "    %this->close();"}
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "read(QtRemoteObjects::QRemoteObjectPacketTypeEnum &, QString &)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QtRemoteObjects::QRemoteObjectPacketTypeEnum %out(QtRemoteObjects::Invalid);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.remoteobjects.IoDeviceBase$@Nullable ReadResult"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(%in){\n"+
                                  "    %out = Java::QtRemoteObjects::IoDeviceBase$ReadResult::newInstance(%env, qtjambi_cast<jobject>(%env, __qt_%1), qtjambi_cast<jobject>(%env, __qt_%2));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "stream()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class IoDevice___"
                quoteBeforeLine: "}// class"
            }
        }
        since: [5, 12]
        until: 5
        unless: "QTJAMBI_NO_REMOTEOBJECTS_PRIVATE"
    }
    
    ObjectType{
        name: "QtROIoDeviceBase"
        InjectCode{
            target: CodeClass.DestructorFunction
            Text{content: "if (!%this->isClosing())\n"+
                          "    %this->close();"}
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "read(QtRemoteObjects::QRemoteObjectPacketTypeEnum &, QString &)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QtRemoteObjects::QRemoteObjectPacketTypeEnum %out(QtRemoteObjects::Invalid);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.remoteobjects.QtROIoDeviceBase$@Nullable ReadResult"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(%in){\n"+
                                  "    %out = Java::QtRemoteObjects::QtROIoDeviceBase$ReadResult::newInstance(%env, qtjambi_cast<jobject>(%env, __qt_%1), qtjambi_cast<jobject>(%env, __qt_%2));\n"+
                                  "}"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class IoDevice___"
                quoteBeforeLine: "}// class"
            }
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "ServerIoDevice"
        InjectCode{
            target: CodeClass.DestructorFunction
            until: [5, 11]
            Text{content: "if (!%this->m_isClosing)\n"+
                          "    %this->close();"}
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            until: [5, 12, 0]
        }
        ModifyFunction{
            signature: "read(QtRemoteObjects::QRemoteObjectPacketTypeEnum &, QString &)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QtRemoteObjects::QRemoteObjectPacketTypeEnum %out(QtRemoteObjects::Invalid);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.remoteobjects.ServerIoDevice$@Nullable ReadResult"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(%in){\n"+
                                  "    jclass cls = JavaAPI::resolveClass(%env, \"io/qt/remoteobjects/ServerIoDevice$ReadResult\");\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "    jmethodID constructor = JavaAPI::resolveMethod(%env, \"<init>\", \"(Lio/qt/remoteobjects/QtRemoteObjects$QRemoteObjectPacketTypeEnum;Ljava/lang/String;)V\", cls, false);\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "    %out = %env->NewObject(cls, constructor, qtjambi_cast<jobject>(%env, __qt_%1), qtjambi_cast<jobject>(%env, __qt_%2));\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
            until: [5, 12, 0]
        }
        InjectCode{
            until: [5, 12, 0]
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class IoDevice___"
                quoteBeforeLine: "}// class"
            }
        }
        until: 5
        unless: "QTJAMBI_NO_REMOTEOBJECTS_PRIVATE"
    }
    
    ObjectType{
        name: "QtROServerIoDevice"
        since: [6, 2]
    }
    
    ObjectType{
        name: "ClientIoDevice"
        ModifyField{
            name: "m_dataStream"
            read: false
            write: false
        }
        InjectCode{
            target: CodeClass.DestructorFunction
            until: [5, 11]
            Text{content: "if (!%this->m_isClosing)\n"+
                          "    %this->close();"}
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            until: [5, 12, 0]
        }
        ModifyFunction{
            signature: "read(QtRemoteObjects::QRemoteObjectPacketTypeEnum &, QString &)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QtRemoteObjects::QRemoteObjectPacketTypeEnum %out(QtRemoteObjects::Invalid);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.remoteobjects.ClientIoDevice$@Nullable ReadResult"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(%in){\n"+
                                  "    jclass cls = JavaAPI::resolveClass(%env, \"io/qt/remoteobjects/ClientIoDevice$ReadResult\");\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "    jmethodID constructor = JavaAPI::resolveMethod(%env, \"<init>\", \"(Lio/qt/remoteobjects/QtRemoteObjects$QRemoteObjectPacketTypeEnum;Ljava/lang/String;)V\", cls, false);\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "    %out = %env->NewObject(cls, constructor, qtjambi_cast<jobject>(%env, __qt_%1), qtjambi_cast<jobject>(%env, __qt_%2));\n"+
                                  "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
            until: [5, 12, 0]
        }
        InjectCode{
            until: [5, 12, 0]
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class IoDevice___"
                quoteBeforeLine: "}// class"
            }
        }
        until: 5
        unless: "QTJAMBI_NO_REMOTEOBJECTS_PRIVATE"
    }
    
    ObjectType{
        name: "QtROClientIoDevice"
        since: [6, 2]
    }
    
    ValueType{
        name: "QRemoteObjectSourceLocationInfo"
    }
    
    InterfaceType{
        name: "QRemoteObjectPendingCall"
        isValue: true
        ModifyFunction{
            signature: "waitForFinished(int)"
            ModifyArgument{
                index: 1
                RemoveDefaultExpression{}
            }
        }
        InjectCode{
            target: CodeClass.JavaInterface
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiRemoteObjects.java"
                quoteAfterLine: "class QRemoteObjectPendingCall___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QRemoteObjectPendingReply"
        isGeneric: true
        generate: false
    }
    
    ObjectType{
        name: "QRemoteObjectPendingCallWatcher"
        InterfacePolymorphy{
            interfaceType: "QRemoteObjectPendingCall"
            condition: "false"
        }
    }
    
    GlobalFunction{
        signature: "qRegisterRemoteObjectsClient<T>(QString)"
        targetType: "QtRemoteObjects"
        Instantiation{
            proxyCall: "qtjambi_qRegisterRemoteObjectsClient"
            until: [5, 15]
            unless: "QTJAMBI_NO_REMOTEOBJECTS_PRIVATE"
            Argument{
                type: "char"
            }
            AddArgument{
                name: "type"
                type: "java.lang.Class<? extends io.qt.remoteobjects.ClientIoDevice>"
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qRegisterRemoteObjectsClient"
            since: 6
            Argument{
                type: "char"
            }
            AddArgument{
                name: "type"
                type: "java.lang.Class<? extends io.qt.remoteobjects.QtROClientIoDevice>"
            }
        }
    }
    
    GlobalFunction{
        signature: "QT_REMOTEOBJECT()"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "QT_REMOTEOBJECT_IO()"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "QT_REMOTEOBJECT_MODELS()"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "qRegisterRemoteObjectsServer<T>(QString)"
        targetType: "QtRemoteObjects"
        Instantiation{
            proxyCall: "qtjambi_qRegisterRemoteObjectsServer"
            until: [5, 15]
            unless: "QTJAMBI_NO_REMOTEOBJECTS_PRIVATE"
            Argument{
                type: "char"
            }
            AddArgument{
                name: "type"
                type: "java.lang.Class<? extends io.qt.remoteobjects.QConnectionAbstractServer>"
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qRegisterRemoteObjectsServer"
            since: 6
            Argument{
                type: "char"
            }
            AddArgument{
                name: "type"
                type: "java.lang.Class<? extends io.qt.remoteobjects.QConnectionAbstractServer>"
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QAbstractItemModelReplica::QAbstractItemModelReplica', unmatched parameter type 'QAbstractItemModelReplicaImplementation*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QRemoteObjectHostBase::enableRemoting', unmatched parameter type 'const SourceApiMap*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping field 'QRemoteObjectReplica::d_impl' with unmatched type 'QSharedPointer<QReplicaImplementationInterface>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QRemoteObjectPendingCall::QRemoteObjectPendingCall', unmatched parameter type 'QRemoteObjectPendingCallData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'initialize()' in 'QRemoteObjectRegistry'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature '' for function modification in 'QRemoteObjectRegistry' not found. Possible candidates:"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'operator*' for function modification in 'QtRemoteObjects' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'operator=(*)' for function modification in '*' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QRemoteObjectPendingCall."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QRemoteObjectHostBase' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcHostSideConnection' in '*' but not both."}
}
