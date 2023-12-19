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
    packageName: "io.qt.multimedia"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtMultimedia"
    module: "qtjambi.multimedia"
    description: "Classes for audio, video, radio and camera functionality."
    LoadTypeSystem{name: "QtGuiRhi"; since: 6.6}

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QtMultimedia();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_QtMultimedia();"}
    }

    RequiredLibrary{
        name: "QtMultimediaGstTools"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
        until: 5
    }

    RequiredLibrary{
        name: "QtMultimediaWidgets"
        mode: RequiredLibrary.ProvideOnly
        until: 5
    }
    
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
    
    Rejection{
        className: "QAbstractVideoBuffer"
        since: 6
    }
    
    Rejection{
        className: "QImageCapture"
        functionName: "platformImageCapture"
        since: [6, 3]
    }
    
    Rejection{
        className: "QMediaRecorder"
        functionName: "platformRecoder"
        since: [6, 3]
    }
    
    Rejection{
        className: "QAudioFrame"
    }
    
    Rejection{
        className: "QAudioDevicePrivate"
    }
    
    Rejection{
        className: "QPlatformVideoSink"
    }
    
    Rejection{
        className: "QPlatformMediaCaptureSession"
    }
    
    Rejection{
        className: "QPlatformMediaRecorder"
    }
    
    Rejection{
        className: ""
        functionName: "qmediacontrol_iid"
        until: [5, 15]
    }
    
    NamespaceType{
        name: "QAudio"
        Include{
            fileName: "qaudio.h"
            location: Include.Global
        }
        EnumType{
            name: "Error"
        }

        EnumType{
            name: "Mode"
        }

        EnumType{
            name: "State"
        }

        EnumType{
            name: "Role"
        }

        EnumType{
            name: "VolumeScale"
        }
    }
    
    NamespaceType{
        name: "QMultimedia"
        EnumType{name: "AvailabilityStatus"}
        EnumType{name: "EncodingMode"}
        EnumType{name: "EncodingQuality"}
        EnumType{name: "SupportEstimate"}
        until: 5
    }
    
    EnumType{
        name: "QVideoSurfaceFormat::Direction"
        until: 5
    }
    
    EnumType{
        name: "QVideoSurfaceFormat::YCbCrColorSpace"
        until: 5
    }
    
    EnumType{
        name: "QAudioDecoder::Error"
    }
    
    EnumType{
        name: "QAudioDecoder::State"
    }
    
    EnumType{
        name: "QCameraExposure::ExposureMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraExposure::FlashMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraExposure::MeteringMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraExposureControl::ExposureParameter"
        until: 5
    }
    
    EnumType{
        name: "QCameraFeedbackControl::EventType"
        until: 5
    }
    
    EnumType{
        name: "QCameraFocus::FocusMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraFocus::FocusPointMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraFocusZone::FocusZoneStatus"
        until: 5
    }
    
    EnumType{
        name: "QCameraImageCapture::CaptureDestination"
        until: 5
    }
    
    EnumType{
        name: "QCameraImageCapture::DriveMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraImageCapture::Error"
        until: 5
    }
    
    EnumType{
        name: "QCameraImageProcessing::WhiteBalanceMode"
        until: 5
    }
    
    EnumType{
        name: "QCameraImageProcessing::ColorFilter"
        until: 5
    }
    
    EnumType{
        name: "QCameraImageProcessingControl::ProcessingParameter"
        until: 5
    }
    
    EnumType{
        name: "QCameraViewfinderSettingsControl::ViewfinderParameter"
        until: 5
    }
    
    EnumType{
        name: "QMediaPlayer::Error"
    }
    
    EnumType{
        name: "QMediaPlayer::Flag"
    }
    
    EnumType{
        name: "QMediaPlayer::PlaybackState"
    }
    
    EnumType{
        name: "QMediaPlayer::MediaStatus"
    }
    
    EnumType{
        name: "QMediaPlayer::State"
    }
    
    EnumType{
        name: "QMediaPlayer::Loops"
    }
    
    EnumType{
        name: "QMediaPlaylist::Error"
        until: 5
    }
    
    EnumType{
        name: "QMediaPlaylist::PlaybackMode"
        until: 5
    }
    
    EnumType{
        name: "QMediaRecorder::Error"
    }
    
    EnumType{
        name: "QMediaRecorder::State"
    }
    
    EnumType{
        name: "QMediaRecorder::Status"
    }
    
    EnumType{
        name: "QMediaRecorder::EncodingMode"
    }
    
    EnumType{
        name: "QMediaRecorder::Quality"
    }
    
    EnumType{
        name: "QMediaRecorder::RecorderState"
    }
    
    EnumType{
        name: "QMediaStreamsControl::StreamType"
        until: 5
    }
    
    EnumType{
        name: "QRadioData::Error"
        until: 5
    }
    
    EnumType{
        name: "QRadioData::ProgramType"
        until: 5
    }
    
    EnumType{
        name: "QRadioTuner::Band"
        until: 5
    }
    
    EnumType{
        name: "QRadioTuner::Error"
        until: 5
    }
    
    EnumType{
        name: "QRadioTuner::SearchMode"
        until: 5
    }
    
    EnumType{
        name: "QRadioTuner::State"
        until: 5
    }
    
    EnumType{
        name: "QRadioTuner::StereoMode"
        until: 5
    }
    
    EnumType{
        name: "QSound::Loop"
        until: 5
    }
    
    EnumType{
        name: "QSoundEffect::Loop"
    }
    
    EnumType{
        name: "QSoundEffect::Status"
    }
    
    EnumType{
        name: "QCameraControl::PropertyChangeType"
        until: 5
    }
    
    ValueType{
        name: "QVideoFrame"
        EnumType{
            name: "HandleType"
        }

        EnumType{
            name: "MapMode"
        }

        Rejection{
            functionName: "videoBuffer"
            since: 6
        }

        EnumType{
            name: "FieldType"
        }

        EnumType{
            name: "PixelFormat"
        }

        EnumType{
            name: "RotationAngle"
        }

        ValueType{
            name: "PaintOptions"
            EnumType{name: "PaintFlag"}
            since: [6, 2]
        }
        ModifyFunction{
            signature: "operator=(const QVideoFrame &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "bits()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "bits(int)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "bits(int)const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "bits(int) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "byte[]"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %env->NewByteArray(__qt_this->mappedBytes(%1));\n"+
                                  "%env->SetByteArrayRegion(%out, 0, __qt_this->mappedBytes(%1), reinterpret_cast<const jbyte *>(%in));"}
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "bits(int)"
            rename: "buffer"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "__qt_this->mappedBytes(%1)"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "bits() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "byte[]"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %env->NewByteArray(__qt_this->mappedBytes());\n"+
                                  "%env->SetByteArrayRegion(%out, 0, __qt_this->mappedBytes(), reinterpret_cast<const jbyte *>(%in));"}
                }
            }
            until: 5
        }
    }
    
    ValueType{
        name: "QAudioBuffer"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }

        Rejection{
            className: "StereoFrame"
        }
        Rejection{
            className: "StereoFrameDefault"
        }
        ModifyFunction{
            signature: "constData<T>()const"
            Instantiation{
                Argument{
                    type: "char"
                }
                rename: "dataAsReadOnlyBuffer"
                ModifyArgument{
                    index: 0
                    AsBuffer{
                        lengthExpression: "__qt_this->byteCount()"
                    }
                    DefineOwnership{
                        codeClass: CodeClass.Native
                        ownership: Ownership.Dependent
                    }
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "data<T>()const"
            Instantiation{
                rename: "dataAsArray"
                Argument{
                    type: "char"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "byte[]"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = %env->NewByteArray(__qt_this->byteCount());\n"+
                                      "%env->SetByteArrayRegion(%out, 0, __qt_this->byteCount(), reinterpret_cast<const jbyte*>(%in));"}
                    }
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "data<T>()"
            Instantiation{
                Argument{
                    type: "char"
                }
                rename: "dataAsBuffer"
                ModifyArgument{
                    index: 0
                    AsBuffer{
                        lengthExpression: "__qt_this->byteCount()"
                    }
                    DefineOwnership{
                        codeClass: CodeClass.Native
                        ownership: Ownership.Dependent
                    }
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "data() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "constData() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "data()"
            remove: RemoveFlag.All
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QAudioBuffer__6_"
                quoteBeforeLine: "}// class"
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QAbstractVideoBuffer"
        EnumType{name: "HandleType"}
        EnumType{name: "MapMode"}
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QAbstractVideoBuffer__"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "map(QAbstractVideoBuffer::MapMode, int*, int*)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %in = 0;\n"+
                                  "int* %out = &%in;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %in = 0;\n"+
                                  "int* %out = &%in;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QAbstractVideoBuffer$MapResult"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject buffer = DataJBuffer(%env, %in, %2).take();\n"+
                                  "QtJambiAPI::registerDependency(%env, buffer, __this_nativeId);\n"+
                                  "jobject %out = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::newInstance(%env, buffer, jint(%3), false);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "uchar* %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "jobject data = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::data(%env, %in);\n"+
                                  "if(%3){\n"+
                                  "*%3 = int(Java::QtMultimedia::QAbstractVideoBuffer$MapResult::bytesPerLine(%env, %in));\n"+
                                  "}\n"+
                                  "if(%2){\n"+
                                  "*%2 = int(%env->GetDirectBufferCapacity(data));\n"+
                                  "}\n"+
                                  "%out = reinterpret_cast<uchar*>(%env->GetDirectBufferAddress(data));\n"+
                                  "}else{\n"+
                                  "if(%2){\n"+
                                  "*%2 = 0;\n"+
                                  "}\n"+
                                  "if(%3){\n"+
                                  "*%3 = 0;\n"+
                                  "}\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "mapPlanes(QAbstractVideoBuffer::MapMode, int *, Array, Array)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %in = 0;\n"+
                                  "int* %out = &%in;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out[4];\n"+
                                  "%out[0] = 0;\n"+
                                  "%out[1] = 0;\n"+
                                  "%out[2] = 0;\n"+
                                  "%out[3] = 0;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "uchar* %out[4];\n"+
                                  "%out[0] = nullptr;\n"+
                                  "%out[1] = nullptr;\n"+
                                  "%out[2] = nullptr;\n"+
                                  "%out[3] = nullptr;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QAbstractVideoBuffer$@NonNull MapResult @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(%in>0){\n"+
                                  "if(%in>4)\n"+
                                  "%in = 4;\n"+
                                  "%out = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::newArray(%env, jsize(%in));\n"+
                                  "for(int i=0; i<%in; ++i){\n"+
                                  "jobject buffer = DataJBuffer(%env, __qt_%4[i], %2).take();\n"+
                                  "jobject element = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::newInstance(%env, buffer, jint(__qt_%3[i]), false);\n"+
                                  "%env->SetObjectArrayElement(%out, jsize(i), element);\n"+
                                  "JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "}\n"+
                                  "}"}
                }
            }
        }
        ModifyField{
            name: "m_type"
            read: false
            rename: "handleType"
        }
        until: 5
    }
    
    ObjectType{
        name: "QAbstractPlanarVideoBuffer"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "map(QAbstractVideoBuffer::MapMode, int *, Array, Array)"
            rename: "mapPlanesImpl"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %in = 0;\n"+
                                  "int* %out = &%in;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out[4];\n"+
                                  "%out[0] = 0;\n"+
                                  "%out[1] = 0;\n"+
                                  "%out[2] = 0;\n"+
                                  "%out[3] = 0;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "uchar* %out[4];\n"+
                                  "%out[0] = nullptr;\n"+
                                  "%out[1] = nullptr;\n"+
                                  "%out[2] = nullptr;\n"+
                                  "%out[3] = nullptr;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QAbstractVideoBuffer$@NonNull MapResult @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobjectArray %out = nullptr;\n"+
                                  "if(%in>0){\n"+
                                  "if(%in>4)\n"+
                                  "%in = 4;\n"+
                                  "%out = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::newArray(%env, jsize(%in));\n"+
                                  "for(int i=0; i<%in; ++i){\n"+
                                  "jobject buffer = DataJBuffer(%env, __qt_%4[i], %2).take();\n"+
                                  "jobject element = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::newInstance(%env, buffer, jint(__qt_%3[i]), false);\n"+
                                  "%env->SetObjectArrayElement(%out, jsize(i), element);\n"+
                                  "JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                                  "}\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "int %out = 0;\n"+
                                  "%3[0] = 0;\n"+
                                  "%3[1] = 0;\n"+
                                  "%3[2] = 0;\n"+
                                  "%3[3] = 0;\n"+
                                  "%4[0] = nullptr;\n"+
                                  "%4[1] = nullptr;\n"+
                                  "%4[2] = nullptr;\n"+
                                  "%4[3] = nullptr;\n"+
                                  "if(%in){\n"+
                                  "%out = qMin(4, int(%env->GetArrayLength(%in)));\n"+
                                  "for(int i=0; i<%out; ++i){\n"+
                                  "jobject element = %env->GetObjectArrayElement(%in, jsize(i));\n"+
                                  "if(!element){\n"+
                                  "%out = i;\n"+
                                  "break;\n"+
                                  "}\n"+
                                  "jobject data = Java::QtMultimedia::QAbstractVideoBuffer$MapResult::data(%env, element);\n"+
                                  "if(!data){\n"+
                                  "%out = i;\n"+
                                  "break;\n"+
                                  "}\n"+
                                  "%3[i] = int(Java::QtMultimedia::QAbstractVideoBuffer$MapResult::bytesPerLine(%env, element));\n"+
                                  "if(%2){\n"+
                                  "int capacity = int(%env->GetDirectBufferCapacity(data));\n"+
                                  "if(capacity<=0){\n"+
                                  "%out = 0;\n"+
                                  "%3[0] = 0;\n"+
                                  "%3[1] = 0;\n"+
                                  "%3[2] = 0;\n"+
                                  "%3[3] = 0;\n"+
                                  "%4[0] = nullptr;\n"+
                                  "%4[1] = nullptr;\n"+
                                  "%4[2] = nullptr;\n"+
                                  "%4[3] = nullptr;\n"+
                                  "break;\n"+
                                  "}\n"+
                                  "if(i>0){\n"+
                                  "*%2 = qMin(*%2, capacity);\n"+
                                  "}else{\n"+
                                  "*%2 = capacity;\n"+
                                  "}\n"+
                                  "}\n"+
                                  "%4[i] = reinterpret_cast<uchar*>(%env->GetDirectBufferAddress(data));\n"+
                                  "}\n"+
                                  "}else{\n"+
                                  "if(%2){\n"+
                                  "*%2 = 0;\n"+
                                  "}\n"+
                                  "}"}
                }
            }
        }
    }
    
    ValueType{
        name: "QAudioDeviceInfo"
        until: 5
    }
    
    ValueType{
        name: "QAudioFormat"

        EnumType{
            name: "AudioChannelPosition"
            since: [6, 2]
        }

        EnumType{
            name: "ChannelConfig"
            extensible: true
            since: [6, 2]
        }
        EnumType{
            name: "Endian"
        }
        EnumType{
            name: "SampleType"
        }
        EnumType{
            name: "SampleFormat"
        }
        ModifyFunction{
            signature: "channelConfig<Args...>(Args)"
            remove: RemoveFlag.All
            since: 6
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QAudioFormat__"
                quoteBeforeLine: "}// class"
                since: [6, 2]
            }
        }
        ModifyFunction{
            signature: "normalizedSampleValue(const void*)const"
            ModifyArgument{
                index: 1
                AsBuffer{}
            }
            since: [6, 2]
        }
    }
    
    ValueType{
        name: "QVideoSurfaceFormat"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
        until: 5
    }
    
    ValueType{
        name: "QAudioEncoderSettings"
    }
    
    ValueType{
        name: "QCameraFocusZone"
    }
    
    ValueType{
        name: "QImageEncoderSettings"
    }
    
    ValueType{
        name: "QMediaContent"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QMediaResource"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QMediaTimeInterval"
    }
    
    ValueType{
        name: "QMediaTimeRange"
        noImplicitConstructors: true
        ValueType{
            name: "Interval"
            since: [6, 2]
        }
        ModifyFunction{
            signature: "operator+=(const QMediaTimeRange &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMediaTimeRange &)"
            rename: "subtract"
        }
    }
    
    ValueType{
        name: "QVideoEncoderSettings"
    }
    
    ValueType{
        name: "QMediaServiceProviderHint"
        noImplicitConstructors: true

        EnumType{
            name: "Feature"
        }

        EnumType{
            name: "Type"
        }
        until: 5
    }
    
    ValueType{
        name: "QCameraViewfinderSettings"
    }
    
    ObjectType{
        name: "QAbstractVideoSurface"
        EnumType{name: "Error"}
        until: 5
    }
    
    ObjectType{
        name: "QAudioOutput"
        ModifyFunction{
            signature: "QAudioOutput(const QAudioFormat &,QObject*)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new QAudioFormat()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "QAudioOutput(const QAudioDeviceInfo &,const QAudioFormat &,QObject*)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new QAudioFormat()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "notify()"
            rename: "notifySignal"
            until: 5
        }
    }
    
    ObjectType{
        name: "QAudioDecoder"
        ModifyFunction{
            signature: "setSourceDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QAudioDecoderControl"
        ModifyFunction{
            signature: "setSourceDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QAudioEncoderSettingsControl"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedSampleRates(const QAudioEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Integer>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<int> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<int>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QAudioInput"
        ModifyFunction{
            signature: "QAudioInput(const QAudioFormat &,QObject*)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new QAudioFormat()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "QAudioInput(const QAudioDeviceInfo &,const QAudioFormat &,QObject*)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new QAudioFormat()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "notify()"
            rename: "notifySignal"
            until: 5
        }
    }
    
    ObjectType{
        name: "QAudioInputSelectorControl"
    }
    
    ObjectType{
        name: "QAudioOutputSelectorControl"
    }
    
    ObjectType{
        name: "QAudioProbe"
        ModifyFunction{
            signature: "setSource(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSource"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSource(QMediaRecorder*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSource"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QAudioRecorder"
    }
    
    ObjectType{
        name: "QCamera"
        Rejection{
            functionName: "platformCamera"
            since: [6, 3]
        }
        EnumType{name: "CaptureMode"}
        EnumType{name: "Error"}
        EnumType{name: "LockChangeReason"}
        EnumType{name: "LockStatus"}
        EnumType{name: "LockType"}
        EnumType{name: "State"}
        EnumType{name: "Status"}
        EnumType{name: "Position"}
        EnumType{name: "ExposureMode"}
        EnumType{name: "Feature"}
        EnumType{name: "FlashMode"}
        EnumType{name: "FocusMode"}
        EnumType{name: "WhiteBalanceMode"}
        EnumType{name: "TorchMode"}
        ValueType{name: "FrameRateRange"}
        ModifyFunction{
            signature: "setViewfinder(QAbstractVideoSurface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setViewfinder(QGraphicsVideoItem*)"
            remove: RemoveFlag.All
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        InjectCode{
            until: 5
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QCamera__"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "setViewfinder(QVideoWidget*)"
            rename: "setViewfinderImpl"
            access: Modification.Private
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.core.QObject"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVideoWidget* %out = reinterpret_cast<QVideoWidget*>(qtjambi_cast<QObject*>(%env, %in));"}
                }
                ReferenceCount{
                    variableName: "__rcViewfinder"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
    }
    
    ValueType{
        name: "QCameraInfo"
    }
    
    ObjectType{
        name: "QCameraCaptureBufferFormatControl"
    }
    
    ObjectType{
        name: "QCameraCaptureDestinationControl"
    }
    
    ObjectType{
        name: "QCameraControl"
        until: 5
    }
    
    ObjectType{
        name: "QCameraInfoControl"
    }
    
    ObjectType{
        name: "QCameraExposureControl"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedParameterRange(const QCameraExposureControl::ExposureParameter &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<Object>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<QVariant> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<QVariant>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QCameraFeedbackControl"
        until: 5
    }
    
    ObjectType{
        name: "QCameraFlashControl"
    }
    
    ObjectType{
        name: "QCameraExposure"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedApertures(bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Double>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%1));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<double> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<double>>(%env, __tmp_%in);\n"+
                                  "    if(%1){\n"+
                                  "        *%1 = %env->GetObjectField(%in, Java::QtMultimedia::QMediaService$Result(%env).continuous);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
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
            signature: "supportedIsoSensitivities(bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Integer>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%1));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<int> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<int>>(%env, __tmp_%in);\n"+
                                  "    if(%1){\n"+
                                  "        *%1 = %env->GetObjectField(%in, Java::QtMultimedia::QMediaService$Result(%env).continuous);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
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
            signature: "supportedShutterSpeeds(bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Double>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%1));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<double> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<double>>(%env, __tmp_%in);\n"+
                                  "    if(%1){\n"+
                                  "        *%1 = %env->GetObjectField(%in, Java::QtMultimedia::QMediaService$Result(%env).continuous);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QCameraFocus"
        until: 5
    }
    
    ObjectType{
        name: "QCameraFocusControl"
    }
    
    ObjectType{
        name: "QCameraImageCapture"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedResolutions(const QImageEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<io.qt.core.@NonNull QSize>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<QSize> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<QSize>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QCameraImageCaptureControl"
    }
    
    ObjectType{
        name: "QCameraImageProcessing"
        until: 5
    }
    
    ObjectType{
        name: "QCameraImageProcessingControl"
        until: 5
    }
    
    ObjectType{
        name: "QCameraLocksControl"
    }
    
    ObjectType{
        name: "QCameraViewfinderSettingsControl"
        until: 5
    }
    
    ObjectType{
        name: "QCameraViewfinderSettingsControl2"
        until: 5
    }
    
    ObjectType{
        name: "QCameraZoomControl"
    }
    
    ObjectType{
        name: "QImageEncoderControl"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedResolutions(const QImageEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<io.qt.core.@NonNull QSize>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<QSize> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<QSize>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QMediaAudioProbeControl"
    }
    
    ObjectType{
        name: "QMediaAvailabilityControl"
    }
    
    ObjectType{
        name: "QMediaContainerControl"
    }
    
    ObjectType{
        name: "QMediaControl"
    }
    
    ObjectType{
        name: "QAudioRoleControl"
    }
    
    ObjectType{
        name: "QMediaGaplessPlaybackControl"
    }
    
    ObjectType{
        name: "QMediaNetworkAccessControl"
    }
    
    ObjectType{
        name: "QAbstractAudioDeviceInfo"
    }
    
    ObjectType{
        name: "QAbstractAudioInput"
        ModifyFunction{
            signature: "notify()"
            rename: "notifySignal"
        }
    }
    
    ObjectType{
        name: "QAbstractAudioOutput"
        ModifyFunction{
            signature: "notify()"
            rename: "notifySignal"
        }
    }
    
    ObjectType{
        name: "QMediaObject"
    }
    
    ObjectType{
        name: "QMediaPlayer"
        ModifyFunction{
            signature: "setVideoOutput(QGraphicsVideoItem*)"
            remove: RemoveFlag.All
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setVideoOutput(QAbstractVideoSurface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setPlaylist(QMediaPlaylist*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setMedia(QMediaContent,QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setAudioOutput(QAudioOutput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAudioOutput"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "setSourceDevice(QIODevice*,QUrl)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSourceDevice"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "setVideoOutput(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcVideoOutput"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "setVideoSink(QVideoSink*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcVideoSink"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "setVideoOutput(QVideoWidget*)"
            rename: "setVideoOutputImpl"
            access: Modification.Private
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcVideoOutput"
                    action: ReferenceCount.Set
                }
                ReplaceType{
                    modifiedType: "io.qt.core.@Nullable QObject"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVideoWidget* %out = reinterpret_cast<QVideoWidget*>(qtjambi_cast<QObject*>(%env, %in));"}
                }
            }
            until: 5
        }
        InjectCode{
            until: 5
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QMediaPlayer__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QMediaPlayerControl"
        ModifyFunction{
            signature: "setMedia(QMediaContent,QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QMediaPlaylist"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QMediaRecorder"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedAudioSampleRates(const QAudioEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Integer>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<int> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<int>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "supportedFrameRates(const QVideoEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Double>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<double> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<double>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "supportedResolutions(const QVideoEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<io.qt.core.@NonNull QSize>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<QSize> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<QSize>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QMediaRecorderControl"
    }
    
    ObjectType{
        name: "QMediaService"
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QMediaService___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "requestControl<T>()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "requestControl(const char *)"
            AddTypeParameter{
                name: "T"
                extending: "io.qt.multimedia.QMediaControl"
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "T"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, jclass(%1));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QMediaControl* %out = qtjambi_cast<QMediaControl*>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<T>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = RegistryAPI::mediaControlIID(%env, jclass(%in));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jclass %out = RegistryAPI::classByMediaControlIID(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QMediaStreamsControl"
        until: 5
    }
    
    ObjectType{
        name: "QMediaVideoProbeControl"
    }
    
    ObjectType{
        name: "QMetaDataReaderControl"
    }
    
    ObjectType{
        name: "QMetaDataWriterControl"
    }
    
    ObjectType{
        name: "QRadioData"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        until: 5
    }
    
    ObjectType{
        name: "QRadioDataControl"
    }
    
    ObjectType{
        name: "QRadioTuner"
        until: 5
    }
    
    ObjectType{
        name: "QRadioTunerControl"
    }
    
    ObjectType{
        name: "QSound"
        until: 5
    }
    
    ObjectType{
        name: "QSoundEffect"
    }
    
    ObjectType{
        name: "QVideoDeviceSelectorControl"
    }
    
    ObjectType{
        name: "QVideoEncoderSettingsControl"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "supportedFrameRates(const QVideoEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<@QtPrimitiveType@NonNull Double>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<double> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<double>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
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
            signature: "supportedResolutions(const QVideoEncoderSettings &, bool *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.multimedia.QMediaService$@StrictNonNull ListResult<io.qt.core.@NonNull QSize>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = Java::QtMultimedia::QMediaService$ListResult::newInstance(\n"+
                                  "%env, qtjambi_cast<jobject>(%env, %in), jboolean(%2));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QList<QSize> %out;\n"+
                                  "if(%in){\n"+
                                  "    jobject __tmp_%in = Java::QtMultimedia::QMediaService$Result::result(%env, %in);\n"+
                                  "    if(__tmp_%in)\n"+
                                  "        %out = qtjambi_cast<QList<QSize>>(%env, __tmp_%in);\n"+
                                  "    if(%2){\n"+
                                  "        *%2 = Java::QtMultimedia::QMediaService$Result::continuous(%env, %in);\n"+
                                  "    }\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QVideoProbe"
        ModifyFunction{
            signature: "setSource(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSource"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSource(QMediaRecorder*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSource"
                    condition: "__qt_return_value"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QVideoRendererControl"
        ModifyFunction{
            signature: "setSurface(QAbstractVideoSurface*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSurface"
                    action: ReferenceCount.Set
                    access: Modification.Friendly
                }
            }
        }
    }
    
    ObjectType{
        name: "QVideoWindowControl"
    }
    
    ObjectType{
        name: "QMediaServiceProviderPlugin"
        ModifyFunction{
            signature: "create(const QString&)"
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
    
    ObjectType{
        name: "QAudioSystemPlugin"
        ModifyFunction{
            signature: "createDeviceInfo(const QByteArray&, QAudio::Mode)"
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
        ModifyFunction{
            signature: "createInput(const QByteArray&)"
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
        ModifyFunction{
            signature: "createOutput(const QByteArray&)"
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
    
    ObjectType{
        name: "QGstBufferPoolPlugin"
    }
    
    ObjectType{
        name: "QMediaPlaylistIOPlugin"
    }
    
    ObjectType{
        name: "QMediaResourcePolicyPlugin"
    }
    
    ObjectType{
        name: "QSGVideoNodeFactoryPlugin"
    }
    
    ObjectType{
        name: "QAbstractVideoFilter"
        ModifyFunction{
            signature: "createFilterRunnable()"
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
    
    ObjectType{
        name: "QCustomAudioRoleControl"
    }
    
    ObjectType{
        name: "QVideoFilterRunnable"

        EnumType{
            name: "RunFlag"
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "run(QVideoFrame *, const QVideoSurfaceFormat &, QVideoFilterRunnable::RunFlags)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                invalidateAfterUse: true
            }
        }
        until: 5
    }
    
    InterfaceType{
        name: "QAudioSystemFactoryInterface"
        ModifyFunction{
            signature: "createDeviceInfo(const QByteArray&, QAudio::Mode)"
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
        ModifyFunction{
            signature: "createInput(const QByteArray&)"
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
        ModifyFunction{
            signature: "createOutput(const QByteArray&)"
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
        name: "QDeclarativeVideoBackendFactoryInterface"
    }
    
    InterfaceType{
        name: "QMediaBindableInterface"
        ModifyFunction{
            signature: "setMediaObject(QMediaObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QMediaServiceFeaturesInterface"
    }
    
    InterfaceType{
        name: "QMediaServiceSupportedDevicesInterface"
    }
    
    InterfaceType{
        name: "QMediaServiceSupportedFormatsInterface"
    }
    
    InterfaceType{
        name: "QMediaServiceProviderFactoryInterface"
        ModifyFunction{
            signature: "create(const QString&)"
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
        InjectCode{
            target: CodeClass.JavaInterface
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiMultimedia.java"
                quoteAfterLine: "class QMediaServiceProviderFactoryInterface_java__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    InterfaceType{
        name: "QMediaServiceCameraInfoInterface"
    }
    
    InterfaceType{
        name: "QMediaServiceDefaultDeviceInterface"
    }
    
    ValueType{
        name: "QCameraFormat"
        EnumType{
            name: "Position"
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QCameraDevice"
        since: [6, 2]
    }
    
    ValueType{
        name: "QAudioDevice"
        EnumType{name: "Mode"}
        since: [6, 2]
    }
    
    ValueType{
        name: "QMediaFormat"
        ModifyField{
            name: "audio"
            read: false
            write: false
        }
        ModifyField{
            name: "video"
            read: false
            write: false
        }
        ModifyField{
            name: "fmt"
            read: false
            write: false
        }
        EnumType{
            name: "FileFormat"
            RejectEnumValue{
                name: "LastFileFormat"
            }
        }
        EnumType{
            name: "AudioCodec"
            RejectEnumValue{
                name: "LastAudioCodec"
            }
        }
        EnumType{
            name: "VideoCodec"
            RejectEnumValue{
                name: "LastVideoCodec"
            }
        }
        EnumType{name: "ConversionMode"}
        EnumType{name: "ResolveFlags"}
        since: [6, 2]
    }
    
    ObjectType{
        name: "QAudioSink"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QAudioSource"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QImageCapture"
        EnumType{
            name: "Error"
        }
        EnumType{
            name: "Quality"
        }
        EnumType{
            name: "FileFormat"
            RejectEnumValue{
                name: "LastFileFormat"
            }
        }
        ModifyFunction{
            signature: "imageAvailable(int,QVideoFrame)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Comment
                Text{
                    content: "<p><b>Make sure to dispose frame object at the end of the slot!</b></p>"
                }
            }
        }
        since: [6, 2]
    }

    ObjectType{
        name: "QScreenCapture"
        ModifyFunction{
            signature: "setScreen(QScreen*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScreen"
                    action: ReferenceCount.Set
                }
            }
        }
        EnumType{name: "Error"}
        since: [6, 5]
    }
    
    ObjectType{
        name: "QMediaCaptureSession"
        ModifyFunction{
            signature: "setAudioInput(QAudioInput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAudioInput"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setAudioOutput(QAudioOutput*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAudioOutput"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setCamera(QCamera*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcCamera"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setRecorder(QMediaRecorder*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRecorder"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setImageCapture(QImageCapture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcImageCapture"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setVideoOutput(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcVideoOutput"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setVideoSink(QVideoSink*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcVideoSink"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setScreenCapture(QScreenCapture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScreenCapture"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 5]
        }
        ModifyFunction{
            signature: "setWindowCapture(QWindowCapture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWindowCapture"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 6]
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QVideoSink"
        ModifyFunction{
            signature: "videoFrameChanged(QVideoFrame)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Comment
                Text{
                    content: "<p><b>Make sure to dispose frame object at the end of the slot!</b></p>"
                }
            }
        }
        ModifyFunction{
            signature: "setRhi(QRhi*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRhi"
                    action: ReferenceCount.Set
                }
            }
            since: [6, 6]
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWaveDecoder"
        ModifyFunction{
            signature: "setIODevice(QIODevice *)"
            remove: RemoveFlag.All
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIODevice"
                    action: ReferenceCount.Set
                }
            }
        }
        Rejection{className: "chunk"}
        Rejection{functionName: "peekChunk"}
        since: [6, 2]
    }
    
    ObjectType{
        name: "QMediaDevices"
        since: [6, 2]
    }
    
    ValueType{
        name: "QMediaMetaData"
        EnumType{
            name: "Key"
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QVideoFrameFormat"
        EnumType{name: "PixelFormat"}
        EnumType{name: "Direction"}
        EnumType{name: "YCbCrColorSpace"}
        EnumType{
            name: "ColorTransfer"
            since: [6, 4]
        }
        EnumType{
            name: "ColorRange"
            since: [6, 4]
        }
        EnumType{
            name: "ColorSpace"
            since: [6, 4]
        }
        since: [6, 2]
    }

    ObjectType{
        name: "QWindowCapture"
        since: [6, 6]
    }
    ObjectType{
        name: "QCapturableWindow"
        since: [6, 6]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QAudioBuffer::QAudioBuffer', unmatched parameter type 'QAbstractAudioBuffer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QMediaService::requestControl', unmatched return type 'T'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QAudioBuffer."}
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: skipping function 'QVideoFrame::QVideoFrame', unmatched parameter type 'QAbstractVideoBuffer*'"
        since: [6, 2]
    }
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: skipping function 'QMediaCaptureSession::platformSession', unmatched return type 'QPlatformMediaCaptureSession*'"
        since: [6, 2]
    }
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: skipping function 'QAudioOutput::handle', unmatched return type 'QPlatformAudioOutput*'"
        since: [6, 2]
    }
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: skipping function 'QAudioInput::handle', unmatched return type 'QPlatformAudioInput*'"
        since: [6, 2]
    }
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: skipping function 'QVideoSink::platformVideoSink', unmatched return type 'QPlatformVideoSink*'"
        since: [6, 2]
    }
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: private virtual function '*' in 'QWaveDecoder'"
        since: [6, 2]
    }
    SuppressedWarning{
        text: "WARNING(MetaJavaBuilder) :: Class 'QMediaMetaData' has equals operators but no qHash() function. Hashcode of objects will consistently be 0."
        since: [6, 2]
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QCamera::platformCamera', unmatched return type 'QPlatformCamera*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QImageCapture::platformImageCapture', unmatched return type 'QPlatformImageCapture*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QMediaRecorder::platformRecoder', unmatched return type 'QPlatformMediaRecorder*'"}
}
