/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.texttospeech"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtTextToSpeech"
    module: "qtjambi.texttospeech"
    description: "Provides support for accessibility features such as text-to-speech."

    ObjectType{
        name: "QTextToSpeech"

        EnumType{
            name: "State"
        }
        EnumType{
            name: "BoundaryHint"
            since: [6, 4]
        }

        EnumType{
            name: "ErrorReason"
            since: [6, 4]
        }

        EnumType{
            name: "Capability"
            since: [6, 6]
        }
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
                since: 6.6
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
                since: 6.6
            }
        }

        FunctionalType{
            name: "Prototype2"
            using: "std::function<void(QAudioFormat, QByteArray)>"
            generate: false
            since: 6.6
        }

        FunctionalType{
            name: "Prototype1"
            using: "std::function<void(QAudioBuffer)>"
            generate: false
            since: 6.6
        }

        ModifyFunction{
            signature: "synthesize<Functor>(const QString&,const QtPrivate::ContextTypeForFunctor::ContextType<Functor>*,Functor&&)"
            Instantiation{
                Argument{
                    type: "std::function<void(QAudioFormat, QByteArray)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot2<io.qt.multimedia.@NonNull QAudioFormat, io.qt.core.@NonNull QByteArray>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = [slot = JObjectWrapper(%env, %in)](const QAudioFormat& format, const QByteArray& buffer){\n"+
                                      "                    if(JniEnvironment env{200}){\n"+
                                      "                        jobject _format = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &format, Java::QtMultimedia::QAudioFormat::getClass(env));\n"+
                                      "                        jobject _buffer = qtjambi_cast<jobject>(env, buffer);\n"+
                                      "                        Java::QtCore::QMetaObject$Slot2::invoke(env, slot.object(env), _format, _buffer);\n"+
                                      "                    }\n"+
                                      "                };"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QAudioBuffer)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot1<io.qt.multimedia.@NonNull QAudioBuffer>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = [slot = JObjectWrapper(%env, %in)](const QAudioBuffer& buffer){\n"+
                                      "                    if(JniEnvironment env{200}){\n"+
                                      "                        jobject _buffer = QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &buffer, Java::QtMultimedia::QAudioBuffer::getClass(env));\n"+
                                      "                        Java::QtCore::QMetaObject$Slot1::invoke(env, slot.object(env), _buffer);\n"+
                                      "                    }\n"+
                                      "                };"}
                    }
                }
            }
            since: 6.6
        }
        ModifyFunction{
            signature: "synthesize<Functor>(QString,Functor&&)"
            remove: RemoveFlag.All
            since: 6.6
        }
        ModifyFunction{
            signature: "findVoices<Args...>(Args&&)const"
            remove: RemoveFlag.All
            since: 6.6
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            since: 6.6
            Text{content: String.raw`
/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(){
    io.qt.core.QList<QVoice> voices = allVoices(null);
    return voices;
}

/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(java.lang.@NonNull String name){
    io.qt.core.QList<QVoice> voices = allVoices(new io.qt.core.QLocale(name));
    return voices;
}

/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(io.qt.core.@NonNull QLocale locale){
    io.qt.core.QList<QVoice> voices = allVoices(locale);
    return voices;
}

/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(io.qt.core.QLocale.@NonNull Language language){
    io.qt.core.QList<QVoice> voices = allVoices(new io.qt.core.QLocale(language));
    return voices;
}

/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(io.qt.core.QLocale.@NonNull Language language, io.qt.core.QLocale.@NonNull Country territory){
    io.qt.core.QList<QVoice> voices = allVoices(new io.qt.core.QLocale(language, territory));
    return voices;
}

/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(io.qt.core.QLocale.@NonNull Language language, io.qt.core.QLocale.@NonNull Script script){
    io.qt.core.QList<QVoice> voices = allVoices(new io.qt.core.QLocale(language, script));
    return voices;
}

/**
 * <p>See <code><a href="https://doc.qt.io/qt/qtexttospeech.html#findVoices">QTextToSpeech::<wbr/>findVoices&lt;Args...&gt;(Args...)</a></code></p>
 */
public io.qt.core.QList<QVoice> findVoices(io.qt.core.QLocale.@NonNull Language language, io.qt.core.QLocale.@NonNull Script script, io.qt.core.QLocale.@NonNull Country territory){
    io.qt.core.QList<QVoice> voices = allVoices(new io.qt.core.QLocale(language, script, territory));
    return voices;
}`
            }
        }
    }

    ObjectType{
        name: "QTextToSpeechEngine"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }

    ObjectType{
        name: "QTextToSpeechPlugin"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "createTextToSpeechEngine(const QMap<QString,QVariant> &, QObject *, QString *) const"
            throwing: "CreateException"
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
                              "    Java::QtTextToSpeech::QTextToSpeechPlugin$CreateException::throwNew(%env, %3 QTJAMBI_STACKTRACEINFO );\n"+
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
                              "    if(exn.isInstanceOf(%env, Java::QtTextToSpeech::QTextToSpeechPlugin$CreateException::getClass(%env))){\n"+
                              "        if(%3){\n"+
                              "            jstring message = Java::QtTextToSpeech::QTextToSpeechPlugin$CreateException::getMessage(%env, exn.throwable(%env));\n"+
                              "            *%3 = qtjambi_cast<QString>(%env, message);\n"+
                              "        }\n"+
                              "    }else{\n"+
                              "        exn.raise();\n"+
                              "    }\n"+
                              "}QTJAMBI_TRY_END"}
            }
        }
        InjectCode{
            Text{content: "public static class CreateException extends Exception {\n"+
                          "    private static final long serialVersionUID = 5359890019927868780L;\n"+
                          "    public CreateException(String message) {\n"+
                          "        super(message);\n"+
                          "    }\n"+
                          "}"}
        }
    }

    ValueType{
        name: "QVoice"

        EnumType{
            name: "Age"
        }

        EnumType{
            name: "Gender"
        }
    }
}
