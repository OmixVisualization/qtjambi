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
    packageName: "io.qt.positioning"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtPositioning"
    module: "qtjambi.positioning"
    description: "Provides access to position, satellite and area monitoring classes."

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QtPositioning();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_QtPositioning();"}
    }
    
    ValueType{
        name: "QGeoAreaMonitorInfo"
    }
    
    ValueType{
        name: "QGeoAddress"
    }
    
    ValueType{
        name: "QGeoShape"
        Rejection{
            functionName: "dataStreamOut"
        }
        Rejection{
            functionName: "dataStreamIn"
        }
        EnumType{
            name: "ShapeType"
        }
        isPolymorphicBase: true
        polymorphicIdExpression: "%1->type() == QGeoShape::UnknownType"
        ModifyFunction{
            signature: "toString()const"
            access: Modification.NonFinal
        }
    }
    
    ValueType{
        name: "QGeoCircle"
        noImplicitConstructors: true
        polymorphicIdExpression: "%1->type() == QGeoShape::CircleType"
        ModifyFunction{
            signature: "center()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGeoCircle(QGeoShape)"
            isForcedExplicit: true
        }
    }
    
    ValueType{
        name: "QGeoPath"
        noImplicitConstructors: true
        polymorphicIdExpression: "%1->type() == QGeoShape::PathType"
        ModifyFunction{
            signature: "QGeoPath(QGeoShape)"
            isForcedExplicit: true
        }
    }
    
    ValueType{
        name: "QGeoPolygon"
        noImplicitConstructors: true
        polymorphicIdExpression: "%1->type() == QGeoShape::PolygonType"
        ModifyFunction{
            signature: "QGeoPolygon(QGeoShape)"
            isForcedExplicit: true
        }
    }
    
    ValueType{
        name: "QGeoRectangle"
        noImplicitConstructors: true
        polymorphicIdExpression: "%1->type() == QGeoShape::RectangleType"
        ModifyFunction{
            signature: "QGeoRectangle(QGeoShape)"
            isForcedExplicit: true
        }
        ModifyFunction{
            signature: "center()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator|=(const QGeoRectangle &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator|(const QGeoRectangle &)const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QGeoCoordinate"

        EnumType{
            name: "CoordinateType"
        }

        EnumType{
            name: "CoordinateFormat"
        }
    }
    
    ValueType{
        name: "QGeoLocation"
    }
    
    ValueType{
        name: "QGeoSatelliteInfo"

        EnumType{
            name: "Attribute"
        }

        EnumType{
            name: "SatelliteSystem"
        }
    }
    
    ValueType{
        name: "QGeoPositionInfo"

        EnumType{
            name: "Attribute"
        }
    }
    
    ObjectType{
        name: "QGeoAreaMonitorSource"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "AreaMonitorFeature"
        }
        ModifyFunction{
            signature: "setPositionInfoSource(QGeoPositionInfoSource*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPositionInfoSource"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "createDefaultSource(QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createSource(QString,QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiPositioning.java"
                quoteAfterLine: "class QGeoAreaMonitorSource___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "requestUpdate(const QGeoAreaMonitorInfo &, const char *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "io.qt.core.QMetaObject$AbstractSignal"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out;\n"+
                                  "if(!%in)\n"+
                                  "    JavaException::raiseNullPointerException(%env, \"Parameter 'signal' must not be null.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "jobject containingObject = Java::QtJambi::SignalUtility$AbstractSignal::containingObject(%env, %in);\n"+
                                  "if(jobject __java_this = CoreAPI::javaObject(__this_nativeId, %env)) {\n"+
                                  "    if(!%env->IsSameObject(__java_this, containingObject)){\n"+
                                  "        JavaException::raiseIllegalArgumentException(%env, \"Given signal has to be one of the instance's signals.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "    }\n"+
                                  "}\n"+
                                  "jint signalIndex = Java::QtJambi::SignalUtility$AbstractSignal::methodIndex(%env, %in);\n"+
                                  "QMetaMethod metaSignal = this->metaObject()->method(signalIndex);\n"+
                                  "if(!metaSignal.isValid() || metaSignal.methodType()==QMetaMethod::Signal)\n"+
                                  "    return false;\n"+
                                  "%out = qFlagLocation(QByteArray(\"2\").append(metaSignal.methodSignature()));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QMetaMethod metaSignal = metaObject()->method(%in ? metaObject()->indexOfMethod(%in+1) : -1);\n"+
                                  "if(!metaSignal.isValid() || metaSignal.methodType()!=QMetaMethod::Signal){\n"+
                                  "    __qt_return_value = false;\n"+
                                  "}else{\n"+
                                  "    jobject __java_metaSignal = qtjambi_cast<jobject>(%env, metaSignal);\n"+
                                  "    if(!__java_metaSignal){\n"+
                                  "        __qt_return_value = false;\n"+
                                  "    }else{\n"+
                                  "        jobject %out = Java::QtCore::QMetaMethod::toSignal(%env, __java_metaSignal, __java_this);"}
                }
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                Text{content: "    }\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QGeoPositionInfoSource"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "PositioningMethod"
        }
        ModifyFunction{
            signature: "createDefaultSource(QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createSource(QString,QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createDefaultSource(QMap<QString,QVariant>,QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createSource(QString,QMap<QString,QVariant>,QObject*)"
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
        name: "QGeoSatelliteInfoSource"

        EnumType{
            name: "Error"
        }
        ModifyFunction{
            signature: "createDefaultSource(QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createSource(QString,QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createDefaultSource(QMap<QString,QVariant>,QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createSource(QString,QMap<QString,QVariant>,QObject*)"
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
        name: "QNmeaPositionInfoSource"

        EnumType{
            name: "UpdateMode"
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "parsePosInfoFromNmeaData(const char *, int, QGeoPositionInfo *, bool *)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.positioning.QNmeaPositionInfoSource$@Nullable Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "    %out = Java::QtPositioning::QNmeaPositionInfoSource$Result::newInstance(\n"+
                                  "    %env, qtjambi_cast<jobject>(%env, %3), %4);\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "bool %out = false;\n"+
                                  "if(%in){\n"+
                                  "    if(%3){\n"+
                                  "        jobject __java_%3 = Java::QtPositioning::QNmeaPositionInfoSource$Result::info(%env, %in);\n"+
                                  "        *%3 = qtjambi_cast<QGeoPositionInfo>(%env, __java_%3);\n"+
                                  "    }\n"+
                                  "    if(%4){\n"+
                                  "        *%4 = Java::QtPositioning::QNmeaPositionInfoSource$Result::hasFix(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                AsBuffer{
                    lengthParameter: 2
                    AsArray{}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QGeoPositionInfo %in;\n"+
                                  "QGeoPositionInfo* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
        ModifyFunction{
            signature: "parsePosInfoFromNmeaData(QByteArrayView, QGeoPositionInfo *, bool *)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.positioning.QNmeaPositionInfoSource$@Nullable Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "    %out = Java::QtPositioning::QNmeaPositionInfoSource$Result::newInstance(\n"+
                                  "    %env, qtjambi_cast<jobject>(%env, %2), %3);\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "bool %out = false;\n"+
                                  "if(%in){\n"+
                                  "    if(%2){\n"+
                                  "        jobject __java_%3 = Java::QtPositioning::QNmeaPositionInfoSource$Result::info(%env, %in);\n"+
                                  "        *%2 = qtjambi_cast<QGeoPositionInfo>(%env, __java_%2);\n"+
                                  "    }\n"+
                                  "    if(%3){\n"+
                                  "        *%3 = Java::QtPositioning::QNmeaPositionInfoSource$Result::hasFix(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
                noImplicitCalls: true
                AddImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QGeoPositionInfo %in;\n"+
                                  "QGeoPositionInfo* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
            since: [6,5]
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiPositioning.java"
                quoteAfterLine: "class QNmeaPositionInfoSource___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "setDevice(QIODevice *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(device()!=__rcDevice){\n"+
                              "    __rcDevice = null;\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QNmeaSatelliteInfoSource"

        EnumType{
            name: "SatelliteInfoParseStatus"
        }

        EnumType{
            name: "UpdateMode"
        }
        ModifyFunction{
            signature: "setDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "parseSatelliteInfoFromNmea(const char *, int, QList<QGeoSatelliteInfo> &, QGeoSatelliteInfo::SatelliteSystem &)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                AsBuffer{
                    lengthParameter: 2
                    AsArray{
                        noOffset: true
                    }
                }
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 4
                NoNullPointer{
                }
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "parseSatelliteInfoFromNmea(QByteArrayView, QList<QGeoSatelliteInfo> &, QGeoSatelliteInfo::SatelliteSystem &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                NoNullPointer{
                }
                AsArray{
                    minLength: 1
                }
            }
            since: [6,5]
        }
        ModifyFunction{
            signature: "parseSatellitesInUseFromNmea(const char *, int, QList<int> &)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                AsBuffer{
                    lengthParameter: 2
                    AsArray{
                        noOffset: true
                    }
                }
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "parseSatelliteInfoFromNmea(QByteArrayView,QList<QGeoSatelliteInfo>&,QGeoSatelliteInfo::SatelliteSystem&)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "parseSatellitesInUseFromNmea(QByteArrayView,QList<int>&)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QGeoPositionInfoSourceFactory"
    }
    
    InterfaceType{
        name: "QGeoPositionInfoSourceFactoryV2"
        since: [5, 14]
        until: 5
    }
}
