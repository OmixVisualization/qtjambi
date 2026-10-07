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
    packageName: "io.qt.opcua"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtOpcUa"
    module: "qtjambi.opcua"
    description: "The Qt OPC UA module implements a Qt API to interact with OPC UA servers on top of the open62541 library."

    Rejection{ className: "QOpcUaNodeImpl" }
    Rejection{ className: "QOpcUaClientImpl" }
    Rejection{ className: "QOpcUaHistoryReadResponseImpl" }
    Rejection{ className: "QOpcUaX509ExtensionData" }
    Rejection{ className: "QOpcUaClientImpl" }

    NamespaceType{
        name: "QOpcUa"
        NamespaceType{
            name: "NodeIds"
            hasMetaObject: true
            EnumType{
                name: "Namespace0"
                forceInteger: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
                namespace Java{
                namespace QtOpcUa{
                    QTJAMBI_REPOSITORY_DECLARE_CLASS(QOpcUa$NodeIdComponents,
                                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
                    QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/opcua,QOpcUa$NodeIdComponents,
                                                     QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(SLjava/lang/String;C))
                }
                }
                `}
        }
        InjectCode{
            target: CodeClass.Java
            Text{content: String.raw`
                /**
                 * <p>This class is used to store the result of QOpcUa::nodeIdStringSplit()</p>
                 * @see QOpcUa#nodeIdStringSplit(String)
                 */
                public static final class NodeIdComponents {
                    private NodeIdComponents(short index, String identifier, char identifierType) {
                        this.index = index;
                        this.identifier = identifier;
                        switch(identifierType){
                        case 'i': this.identifierType = Type.Integer; break;
                        case 's': this.identifierType = Type.String; break;
                        case 'g': this.identifierType = Type.GUID; break;
                        case 'b': this.identifierType = Type.ByteString; break;
                        default: this.identifierType = Type.Integer; break;
                        }
                    }
                    public enum Type{Integer, String, GUID, ByteString}
                    public final short index;
                    public final String identifier;
                    public final Type identifierType;
                }
                `}
        }
        ModifyFunction{
            signature: "nodeIdStringSplit(QString,quint16*,QString*,char*)"
            ModifyArgument{
                index: 2
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        quint16 %in{0};
                        quint16* %out = &%in;
                        `}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        QString %in;
                        QString* %out = &%in;
                        `}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        char %in{'i'};
                        char* %out = &%in;
                        `}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.opcua.QOpcUa$@Nullable NodeIdComponents"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in ? Java::QtOpcUa::QOpcUa$NodeIdComponents::newInstance(%env, jshort(%2), qtjambi_cast<jstring>(%env, %3), jchar(%4)) : nullptr;"}
                }
            }
        }
    }

    ValueType{
        name: "QOpcUaAddNodeItem"
        ModifyFunction{
            signature: "nodeAttributesRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaAddReferenceItem"
    }
    ValueType{
        name: "QOpcUaApplicationDescription"
        ModifyFunction{
            signature: "discoveryUrlsRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaApplicationIdentity"
    }
    ValueType{
        name: "QOpcUaApplicationRecordDataType"
    }
    ValueType{
        name: "QOpcUaArgument"
        ModifyFunction{
            signature: "arrayDimensionsRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaAttributeOperand"
        ModifyFunction{
            signature: "browsePathRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaAuthenticationInformation"
    }
    ValueType{
        name: "QOpcUaAxisInformation"
        ModifyFunction{
            signature: "axisStepsRef()"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QOpcUaBinaryDataEncoding"
        ModifyFunction{
            signature: "decode(bool&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "decode<T,OVERLAY>(bool&)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`bool %out = false;`}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = __qt_%1 ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
            Instantiation{
                Argument{
                    type: "bool"
                }
                rename: "decodeBoolean"
                ModifyArgument{
                    index: 0
                    replaceType: "java.lang.@Nullable Boolean"
                }
            }
            Instantiation{
                Argument{
                    type: "QOpcUa::UaStatusCode"
                }
                rename: "decodeUaStatusCode"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.QOpcUa$@Nullable UaStatusCode"
                }
            }

            Instantiation{
                Argument{
                    type: "QString"
                }
                rename: "decodeString"
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "java.lang.@Nullable String"
                        modifiedJniType: "jobject"
                    }
                }
            }

            Instantiation{
                Argument{
                    type: "QString"
                }
                Argument{
                    value: "QOpcUa::Types::NodeId"
                }
                rename: "decodeNodeId"
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "java.lang.@Nullable String"
                        modifiedJniType: "jobject"
                    }
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaQualifiedName"
                }
                rename: "decodeQualifiedName"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaQualifiedName"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaLocalizedText"
                }
                rename: "decodeLocalizedText"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaLocalizedText"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEUInformation"
                }
                rename: "decodeEUInformation"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaEUInformation"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaRange"
                }
                rename: "decodeRange"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaRange"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaComplexNumber"
                }
                rename: "decodeComplexNumber"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaComplexNumber"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDoubleComplexNumber"
                }
                rename: "decodeDoubleComplexNumber"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaDoubleComplexNumber"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaAxisInformation"
                }
                rename: "decodeAxisInformation"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaAxisInformation"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaXValue"
                }
                rename: "decodeXValue"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaXValue"
                }
            }

            Instantiation{
                Argument{
                    type: "QUuid"
                }
                rename: "decodeUuid"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QUuid"
                }
            }

            Instantiation{
                Argument{
                    type: "QByteArray"
                }
                rename: "decodeByteArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QByteArray"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaExpandedNodeId"
                }
                rename: "decodeExpandedNodeId"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaExpandedNodeId"
                }
            }

            Instantiation{
                Argument{
                    type: "QDateTime"
                }
                rename: "decodeDateTime"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QDateTime"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaExtensionObject"
                }
                rename: "decodeExtensionObject"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaExtensionObject"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaArgument"
                }
                rename: "decodeArgument"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaArgument"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaStructureField"
                }
                rename: "decodeStructureField"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaStructureField"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaStructureDefinition"
                }
                rename: "decodeStructureDefinition"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaStructureDefinition"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEnumField"
                }
                rename: "decodeEnumField"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaEnumField"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEnumDefinition"
                }
                rename: "decodeEnumDefinition"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaEnumDefinition"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDiagnosticInfo"
                }
                rename: "decodeDiagnosticInfo"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaDiagnosticInfo"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaVariant"
                }
                rename: "decodeVariant"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaVariant"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDataValue"
                }
                rename: "decodeDataValue"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.opcua.@Nullable QOpcUaDataValue"
                }
            }
        }
        ModifyFunction{
            signature: "decodeArray<T,OVERLAY>(bool&)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`bool %out = false;`}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = __qt_%1 ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
            Instantiation{
                Argument{
                    type: "bool"
                }
                rename: "decodeBooleanArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<java.lang.@NonNull Boolean>"
                }
            }
            Instantiation{
                Argument{
                    type: "QOpcUa::UaStatusCode"
                }
                rename: "decodeUaStatusCodeArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.QOpcUa$@NonNull UaStatusCode>"
                }
            }

            Instantiation{
                Argument{
                    type: "QString"
                }
                rename: "decodeStringArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QStringList"
                }
            }

            Instantiation{
                Argument{
                    type: "QString"
                }
                Argument{
                    value: "QOpcUa::Types::NodeId"
                }
                rename: "decodeNodeIdArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QStringList"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaQualifiedName"
                }
                rename: "decodeQualifiedNameArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaQualifiedName>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaLocalizedText"
                }
                rename: "decodeLocalizedTextArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaLocalizedText>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEUInformation"
                }
                rename: "decodeEUInformationArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaEUInformation>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaRange"
                }
                rename: "decodeRangeArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaRange>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaComplexNumber"
                }
                rename: "decodeComplexNumberArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaComplexNumber>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDoubleComplexNumber"
                }
                rename: "decodeDoubleComplexNumberArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaDoubleComplexNumber>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaAxisInformation"
                }
                rename: "decodeAxisInformationArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaAxisInformation>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaXValue"
                }
                rename: "decodeXValueArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaXValue>"
                }
            }

            Instantiation{
                Argument{
                    type: "QUuid"
                }
                rename: "decodeUuidArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.core.@NonNull QUuid>"
                }
            }

            Instantiation{
                Argument{
                    type: "QByteArray"
                }
                rename: "decodeByteArrayArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.core.@NonNull QByteArray>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaExpandedNodeId"
                }
                rename: "decodeExpandedNodeIdArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaExpandedNodeId>"
                }
            }

            Instantiation{
                Argument{
                    type: "QDateTime"
                }
                rename: "decodeDateTimeArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.core.@NonNull QDateTime>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaExtensionObject"
                }
                rename: "decodeExtensionObjectArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaExtensionObject>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaArgument"
                }
                rename: "decodeArgumentArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaArgument>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaStructureField"
                }
                rename: "decodeStructureFieldArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaStructureField>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaStructureDefinition"
                }
                rename: "decodeStructureDefinitionArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaStructureDefinition>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEnumField"
                }
                rename: "decodeEnumFieldArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaEnumField>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEnumDefinition"
                }
                rename: "decodeEnumDefinitionArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaEnumDefinition>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDiagnosticInfo"
                }
                rename: "decodeDiagnosticInfoArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaDiagnosticInfo>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaVariant"
                }
                rename: "decodeVariantArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaVariant>"
                }
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDataValue"
                }
                rename: "decodeDataValueArray"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@Nullable QList<io.qt.opcua.@NonNull QOpcUaDataValue>"
                }
            }
        }
        ModifyFunction{
            signature: "encode<T,OVERLAY>(T)"
            Instantiation{
                Argument{
                    type: "QString"
                }
                Argument{
                    value: "QOpcUa::Types::NodeId"
                }
                rename: "encodeNodeId"
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "java.lang.@Nullable String"
                        modifiedJniType: "jobject"
                    }
                }
            }
        }
        ModifyFunction{
            signature: "encodeArray<T,OVERLAY>(QList<T>)"
            Instantiation{
                Argument{
                    type: "bool"
                }
                rename: "encodeBooleanArray"
            }
            Instantiation{
                Argument{
                    type: "QOpcUa::UaStatusCode"
                }
                rename: "encodeUaStatusCodeArray"
            }

            Instantiation{
                Argument{
                    type: "QString"
                }
                rename: "encodeStringArray"
            }

            Instantiation{
                Argument{
                    type: "QString"
                }
                Argument{
                    value: "QOpcUa::Types::NodeId"
                }
                rename: "encodeNodeIdArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaQualifiedName"
                }
                rename: "encodeQualifiedNameArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaLocalizedText"
                }
                rename: "encodeLocalizedTextArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEUInformation"
                }
                rename: "encodeEUInformationArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaRange"
                }
                rename: "encodeRangeArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaComplexNumber"
                }
                rename: "encodeComplexNumberArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDoubleComplexNumber"
                }
                rename: "encodeDoubleComplexNumberArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaAxisInformation"
                }
                rename: "encodeAxisInformationArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaXValue"
                }
                rename: "encodeXValueArray"
            }

            Instantiation{
                Argument{
                    type: "QUuid"
                }
                rename: "encodeUuidArray"
            }

            Instantiation{
                Argument{
                    type: "QByteArray"
                }
                rename: "encodeByteArrayArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaExpandedNodeId"
                }
                rename: "encodeExpandedNodeIdArray"
            }

            Instantiation{
                Argument{
                    type: "QDateTime"
                }
                rename: "encodeDateTimeArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaExtensionObject"
                }
                rename: "encodeExtensionObjectArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaArgument"
                }
                rename: "encodeArgumentArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaStructureField"
                }
                rename: "encodeStructureFieldArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaStructureDefinition"
                }
                rename: "encodeStructureDefinitionArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEnumField"
                }
                rename: "encodeEnumFieldArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaEnumDefinition"
                }
                rename: "encodeEnumDefinitionArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDiagnosticInfo"
                }
                rename: "encodeDiagnosticInfoArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaVariant"
                }
                rename: "encodeVariantArray"
            }

            Instantiation{
                Argument{
                    type: "QOpcUaDataValue"
                }
                rename: "encodeDataValueArray"
            }
        }
    }
    ValueType{
        name: "QOpcUaBrowsePathTarget"
        ModifyFunction{
            signature: "targetIdRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaBrowseRequest"
    }
    ValueType{
        name: "QOpcUaClient"
        EnumType{
            name: "ClientError"
        }
        EnumType{
            name: "ClientState"
        }
        ModifyFunction{
            signature: "qualifiedNameFromNamespaceUri(const QString &, const QString &, bool *) const"
            ModifyArgument{
                index: 0
                replaceType: "io.qt.opcua.@Nullable QOpcUaQualifiedName"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %3 ? qtjambi_cast<jobject>(%env, std::move(%in)) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        bool %in = false;
                        bool* %out = &%in;
                        `}
                }
            }
        }
        ModifyFunction{
            signature: "resolveExpandedNodeId(const QOpcUaExpandedNodeId &, bool *) const"
            ModifyArgument{
                index: 0
                replaceType: "java.lang.@Nullable String"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %2 ? qtjambi_cast<jstring>(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        bool %in = false;
                        bool* %out = &%in;
                        `}
                }
            }
        }
    }
    ValueType{
        name: "QOpcUaComplexNumber"
    }
    ObjectType{
        name: "QOpcUaConnectionSettings"
    }
    ValueType{
        name: "QOpcUaContentFilterElement"
        EnumType{
            name: "FilterOperator"
        }
        ModifyFunction{
            signature: "filterOperandsRef()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(QOpcUaContentFilterElement::FilterOperator)"
            rename: "append"
        }
        ModifyFunction{
            signature: "operator<<(QOpcUaAttributeOperand)"
            rename: "append"
        }
        ModifyFunction{
            signature: "operator<<(QOpcUaElementOperand)"
            rename: "append"
        }
        ModifyFunction{
            signature: "operator<<(QOpcUaLiteralOperand)"
            rename: "append"
        }
        ModifyFunction{
            signature: "operator<<(QOpcUaSimpleAttributeOperand)"
            rename: "append"
        }
    }
    ValueType{
        name: "QOpcUaContentFilterElementResult"
        ModifyFunction{
            signature: "operandStatusCodesRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaDataValue"
    }
    ValueType{
        name: "QOpcUaDeleteReferenceItem"
    }
    ValueType{
        name: "QOpcUaDiagnosticInfo"
        ModifyFunction{
            signature: "innerDiagnosticInfoRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaDoubleComplexNumber"
    }
    ValueType{
        name: "QOpcUaEUInformation"
    }
    ValueType{
        name: "QOpcUaElementOperand"
    }
    ValueType{
        name: "QOpcUaEndpointDescription"
        EnumType{
            name: "MessageSecurityMode"
        }
        ModifyFunction{
            signature: "userIdentityTokensRef()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "serverRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaEnumDefinition"
    }
    ValueType{
        name: "QOpcUaEnumField"
    }
    ValueType{
        name: "QOpcUaErrorState"
        EnumType{
            name: "ConnectionStep"
        }
    }
    ValueType{
        name: "QOpcUaEventFilterResult"
        ModifyFunction{
            signature: "whereClauseResultsRef()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "selectClauseResultsRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaExpandedNodeId"
    }
    ValueType{
        name: "QOpcUaExtensionObject"
        ModifyFunction{
            signature: "QOpcUaExtensionObject(QString,QOpcUaExtensionObject::Encoding)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "Encoding.ByteString"
                }
            }
        }
        ModifyFunction{
            signature: "encodedBodyRef()"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QOpcUaGdsClient"
    }
    ObjectType{
        name: "QOpcUaGenericStructHandler"
        EnumType{
            name: "DataTypeKind"
        }
    }
    ValueType{
        name: "QOpcUaGenericStructValue"
        ModifyFunction{
            signature: "fieldsRef()"
            remove: RemoveFlag.All
        }
    }
    ObjectType{
        name: "QOpcUaHistoryData"
    }
    ValueType{
        name: "QOpcUaHistoryEvent"
    }
    ValueType{
        name: "QOpcUaHistoryReadEventRequest"
    }
    ValueType{
        name: "QOpcUaHistoryReadRawRequest"
    }
    ObjectType{
        name: "QOpcUaHistoryReadResponse"
        EnumType{
            name: "State"
        }
    }
    ObjectType{
        name: "QOpcUaKeyPair"
        EnumType{
            name: "Cipher"
        }
        EnumType{
            name: "KeyType"
        }
        EnumType{
            name: "RsaKeyStrength"
        }
        ModifyFunction{
            signature: "passphraseNeeded(QString &, int, bool)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
                        if(%env->IsSameObject(%in, nullptr))
                            JavaException::raiseNullPointerException(%env, "String argument must not be null." QTJAMBI_STACKTRACEINFO );
                        QString* s = qtjambi_cast<QString*>(%env, %in);
                        QString** %out = &s;`}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jcoreobject>(%env, &%in);"}
                }
            }
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
                AddImplicitCall{type: "java.nio.@NonNull ByteBuffer"}
                AddImplicitCall{type: "java.lang.@NonNull CharSequence"}
                AddImplicitCall{type: "java.lang.@NonNull String"}
                AddImplicitCall{type: "byte @NonNull[]"}
            }
            since: 6.7
        }
    }
    ValueType{
        name: "QOpcUaLiteralOperand"
        ModifyFunction{
            signature: "operator==(QOpcUaLiteralOperand,QOpcUaLiteralOperand)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
    }
    ValueType{
        name: "QOpcUaLocalizedText"
    }
    ValueType{
        name: "QOpcUaMonitoringParameters"
        EnumType{
            name: "MonitoringMode"
        }
        EnumType{
            name: "Parameter"
        }
        EnumType{
            name: "SubscriptionType"
        }
        ValueType{
            name: "DataChangeFilter"
        }
        ValueType{
            name: "EventFilter"
            ModifyFunction{
                signature: "selectClausesRef()"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "whereClauseRef()"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "operator QVariant const()"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "operator<<(QOpcUaContentFilterElement)"
                rename: "append"
            }
            ModifyFunction{
                signature: "operator<<(QOpcUaSimpleAttributeOperand)"
                rename: "append"
            }
        }
    }
    ValueType{
        name: "QOpcUaMultiDimensionalArray"
        ModifyFunction{
            signature: "valueArrayRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaNode"
        ObjectType{
            name: "AttributeMap"
        }
    }
    ValueType{
        name: "QOpcUaNodeCreationAttributes"
    }
    ValueType{
        name: "QOpcUaPkiConfiguration"
    }
    ObjectType{
        name: "QOpcUaPlugin"
        ModifyFunction{
            signature: "createClient(const QVariantMap &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    ObjectType{
        name: "QOpcUaProvider"
        ModifyFunction{
            signature: "createClient(const QString &, const QVariantMap &)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    ValueType{
        name: "QOpcUaQualifiedName"
    }
    ValueType{
        name: "QOpcUaRange"
    }
    ValueType{
        name: "QOpcUaReadItem"
    }
    ValueType{
        name: "QOpcUaReadResult"
    }
    ValueType{
        name: "QOpcUaReferenceDescription"
    }
    ValueType{
        name: "QOpcUaRelativePathElement"
    }
    ValueType{
        name: "QOpcUaSimpleAttributeOperand"
        ModifyFunction{
            signature: "browsePathRef()"
            remove: RemoveFlag.All
        }
    }
    ValueType{
        name: "QOpcUaStructureDefinition"
    }
    ValueType{
        name: "QOpcUaStructureField"
    }
    ValueType{
        name: "QOpcUaUserTokenPolicy"
    }
    ValueType{
        name: "QOpcUaVariant"
        EnumType{
            name: "ValueType"
        }
        since: 6.7
    }
    ValueType{
        name: "QOpcUaWriteItem"
    }
    ValueType{
        name: "QOpcUaWriteResult"
    }
    ObjectType{
        name: "QOpcUaX509CertificateSigningRequest"
        EnumType{
            name: "Encoding"
        }
        EnumType{
            name: "MessageDigest"
        }
        ModifyFunction{
            signature: "addExtension(QOpcUaX509Extension*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            since: 6.8
        }
    }
    ValueType{
        name: "QOpcUaX509DistinguishedName"
        EnumType{
            name: "Type"
        }
    }
    ValueType{
        name: "QOpcUaX509Extension"
        Rejection{ fieldName: "data" }
    }
    ValueType{
        name: "QOpcUaX509ExtensionBasicConstraints"
    }
    ValueType{
        name: "QOpcUaX509ExtensionExtendedKeyUsage"
    }
    ValueType{
        name: "QOpcUaX509ExtensionKeyUsage"
    }
    ValueType{
        name: "QOpcUaX509ExtensionSubjectAlternativeName"
    }
    ValueType{
        name: "QOpcUaXValue"
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: duplicate enum values: QOpcUa$NodeIds::Namespace0*"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOpcUa*."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Class 'QOpcUa*' has equals operators but no qHash() function. Hashcode of objects will consistently be 0."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Missing instantiations for template method QOpcUaBinaryDataEncoding::encode<T,OVERLAY>(T)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: duplicate enum values: QOpcUa$NodeIds::Namespace0, *"}
}
