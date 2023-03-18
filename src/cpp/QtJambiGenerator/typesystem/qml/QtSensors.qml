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
    packageName: "io.qt.sensors"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtSensors"
    module: "qtjambi.sensors"
    description: "Provides access to sensor hardware and motion gesture recognition."

    ObjectType{
        name: "QSensorGesture"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "gestureSignals() const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "for(int i=0; i<%in.size(); ++i){\n"+
                                  "    if(%in[i].endsWith(\"(QString)\")){\n"+
                                  "        %in[i].replace(\"QString\", \"java.lang.String\");\n"+
                                  "    }else if(!%in[i].endsWith(\"()\")){\n"+
                                  "        const QObject* obj = __qt_this;\n"+
                                  "        int idx = obj->metaObject()->indexOfMethod(qPrintable(%in[i]));\n"+
                                  "        if(idx>=0){\n"+
                                  "            QMetaMethod method = obj->metaObject()->method(idx);\n"+
                                  "            jobject methodObject = qtjambi_cast<jobject>(%env, method);\n"+
                                  "            jstring sig = Java::QtCore::QMetaMethod::methodSignature(%env, methodObject);\n"+
                                  "            %in[i] = qtjambi_cast<QString>(%env, sig);\n"+
                                  "        }else{\n"+
                                  "            %in[i].replace(\"QString\", \"java.lang.String\");\n"+
                                  "        }\n"+
                                  "    }\n"+
                                  "}\n"+
                                  "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QSensorGestureManager"
        ModifyFunction{
            signature: "recognizerSignals(QString) const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "for(int i=0; i<%in.size(); ++i){\n"+
                                  "    if(!%in[i].endsWith(\"()\")){\n"+
                                  "        %in[i].replace(\"QString\", \"java.lang.String\");\n"+
                                  "    }\n"+
                                  "}\n"+
                                  "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        until: 5
    }
    
    InterfaceType{
        name: "QSensorGesturePluginInterface"
        until: 5
    }
    
    ObjectType{
        name: "QSensorGestureRecognizer"
        until: 5
    }
    
    ObjectType{
        name: "QAccelerometer"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }

        EnumType{
            name: "AccelerationMode"
        }
    }
    
    ObjectType{
        name: "QAccelerometerReading"
    }
    
    ObjectType{
        name: "QAltimeter"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QAltimeterReading"
    }
    
    ObjectType{
        name: "QAmbientLightReading"

        EnumType{
            name: "LightLevel"
        }
    }
    
    ObjectType{
        name: "QAmbientLightSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QAmbientTemperatureReading"
    }
    
    ObjectType{
        name: "QAmbientTemperatureSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QCompass"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QCompassReading"
    }
    
    ObjectType{
        name: "QGyroscope"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QGyroscopeReading"
    }
    
    ObjectType{
        name: "QDistanceSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QDistanceReading"
    }
    
    ObjectType{
        name: "QHolsterReading"
    }
    
    ObjectType{
        name: "QHolsterSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QHumidityReading"
    }
    
    ObjectType{
        name: "QHumiditySensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QIRProximityReading"
    }
    
    ObjectType{
        name: "QIRProximitySensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QLidReading"
    }
    
    ObjectType{
        name: "QLidSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QMagnetometer"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QMagnetometerReading"
    }
    
    ObjectType{
        name: "QOrientationSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QOrientationReading"

        EnumType{
            name: "Orientation"
        }
    }
    
    ObjectType{
        name: "QPressureReading"
    }
    
    ObjectType{
        name: "QPressureSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QProximityReading"
    }
    
    ObjectType{
        name: "QProximitySensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QRotationReading"
    }
    
    ObjectType{
        name: "QRotationSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QTapReading"

        EnumType{
            name: "TapDirection"
        }
    }
    
    ObjectType{
        name: "QTapSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QTiltReading"
    }
    
    ObjectType{
        name: "QTiltSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
    }
    
    ObjectType{
        name: "QSensor"

        EnumType{
            name: "AxesOrientationMode"
        }

        EnumType{
            name: "Feature"
        }
        ModifyFunction{
            signature: "addFilter(QSensorFilter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFilters"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeFilter(QSensorFilter*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFilters"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "reading()const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QSensorReading"
        ModifyFunction{
            signature: "QSensorReading(QObject *, QSensorReadingPrivate *)"
            InjectCode{
                target: CodeClass.Native
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "QSensorReadingPrivate * __qt_%2 = nullptr;"}
            }
            ModifyArgument{
                index: 2
                RemoveArgument{}
            }
        }
    }
    
    ObjectType{
        name: "QLightReading"
        since: [5, 15]
    }
    
    ObjectType{
        name: "QLightSensor"
        ModifyField{
            name: "type"
            read: false
        }
        ModifyField{
            name: "sensorType"
            read: false
        }
        since: [5, 15]
    }
    
    ValueType{
        name: "qoutputrange"
        javaName: "QOutputRange"
    }
    
    ObjectType{
        name: "QSensorBackend"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setDataRates(const QSensor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setReading<T>(T*)"
            Instantiation{
                proxyCall: "qtjambi_setReading"
                Argument{
                    type: "QSensorReading"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                    NoNullPointer{}
                }
                ModifyArgument{
                    index: 1
                    ReferenceCount{
                        action: ReferenceCount.Ignore
                    }
                    NoNullPointer{}
                    ReplaceType{
                        modifiedType: "T"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "jobject %out = %in;"}
                    }
                }
                AddTypeParameter{
                    name: "T"
                    extending: "io.qt.sensors.QSensorReading"
                }
            }

            Instantiation{
                proxyCall: "qtjambi_setReading"
                Argument{
                    type: "QObject"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                    NoNullPointer{}
                }
                ModifyArgument{
                    index: 1
                    NoNullPointer{}
                    ReferenceCount{
                        action: ReferenceCount.Ignore
                    }
                    ReplaceType{
                        modifiedType: "java.lang.Class<T>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "jclass %out = %in;"}
                    }
                }
                AddTypeParameter{
                    name: "T"
                    extending: "io.qt.sensors.QSensorReading"
                }
            }
        }
    }
    
    ObjectType{
        name: "QSensorManager"
        ModifyFunction{
            signature: "createBackend(QSensor *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "registerBackend(const QByteArray &, const QByteArray &, QSensorBackendFactory *)"
            ModifyArgument{
                index: 3
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: "io.qt.core.QPair<io.qt.core.QByteArray,io.qt.core.QByteArray> key = new io.qt.core.QPair<>(%1, %2);\n"+
                              "__rcBackends.put(key, %3);"}
            }
        }
        ModifyFunction{
            signature: "unregisterBackend(const QByteArray &, const QByteArray &)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "io.qt.core.QPair<io.qt.core.QByteArray,io.qt.core.QByteArray> key = new io.qt.core.QPair<>(%1, %2);\n"+
                              "__rcBackends.remove(key);"}
            }
        }
        InjectCode{
            Text{content: "private final static java.util.Map<io.qt.core.QPair<io.qt.core.QByteArray,io.qt.core.QByteArray>,QSensorBackendFactory> __rcBackends = java.util.Collections.synchronizedMap(new java.util.HashMap<>());"}
        }
    }
    
    InterfaceType{
        name: "QLightFilter"
        since: [5, 15]
    }
    
    InterfaceType{
        name: "QSensorFilter"
        ModifyField{
            name: "m_sensor"
            read: true
            write: false
            rename: "sensor"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcSensor"
            }
        }
        ModifyFunction{
            signature: "setSensor(QSensor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSensor"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    InterfaceType{
        name: "QAccelerometerFilter"
    }
    
    InterfaceType{
        name: "QAltimeterFilter"
    }
    
    InterfaceType{
        name: "QAmbientLightFilter"
    }
    
    InterfaceType{
        name: "QAmbientTemperatureFilter"
    }
    
    InterfaceType{
        name: "QCompassFilter"
    }
    
    InterfaceType{
        name: "QDistanceFilter"
    }
    
    InterfaceType{
        name: "QGyroscopeFilter"
    }
    
    InterfaceType{
        name: "QHolsterFilter"
    }
    
    InterfaceType{
        name: "QHumidityFilter"
    }
    
    InterfaceType{
        name: "QIRProximityFilter"
    }
    
    InterfaceType{
        name: "QLidFilter"
    }
    
    InterfaceType{
        name: "QMagnetometerFilter"
    }
    
    InterfaceType{
        name: "QOrientationFilter"
    }
    
    InterfaceType{
        name: "QPressureFilter"
    }
    
    InterfaceType{
        name: "QProximityFilter"
    }
    
    InterfaceType{
        name: "QRotationFilter"
    }
    
    InterfaceType{
        name: "QTapFilter"
    }
    
    InterfaceType{
        name: "QTiltFilter"
    }
    
    InterfaceType{
        name: "QSensorBackendFactory"
        ModifyFunction{
            signature: "createBackend(QSensor*)"
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
        }
    }
    
    InterfaceType{
        name: "QSensorChangesInterface"
    }
    
    InterfaceType{
        name: "QSensorPluginInterface"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'filter(QSensorReading *)' in '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QSensorReading::d_ptr', unmatched return type '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QSensorReading' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type qoutputrange."}
}
