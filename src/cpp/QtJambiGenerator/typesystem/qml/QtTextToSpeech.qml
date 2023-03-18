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
    packageName: "io.qt.texttospeech"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtTextToSpeech"
    module: "qtjambi.texttospeech"
    description: "Provides support for accessibility features such as text-to-speech."

    EnumType{
        name: "QTextToSpeech::State"
    }
    
    EnumType{
        name: "QTextToSpeech::BoundaryHint"
        since: [6, 4]
    }
    
    EnumType{
        name: "QTextToSpeech::ErrorReason"
        since: [6, 4]
    }
    
    ObjectType{
        name: "QTextToSpeech"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
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
                              "    if(Java::QtTextToSpeech::QTextToSpeechPlugin$CreateException::isInstanceOf(%env, exn.object())){\n"+
                              "        if(%3){\n"+
                              "            jstring message = Java::QtTextToSpeech::QTextToSpeechPlugin$CreateException::getMessage(%env, exn.object());\n"+
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
    
    EnumType{
        name: "QVoice::Age"
    }
    
    EnumType{
        name: "QVoice::Gender"
    }
    
    ValueType{
        name: "QVoice"
    }
}
